#include "pvz/GameWorld/GameWorld.hpp"

#include "pvz/GameObject/UIObjects.hpp"
#include "pvz/GameObject/ZombieObjects.hpp"

namespace {

// Base interface starts with one regular zombie card and this sun amount.
constexpr int INITIAL_SUN_AMOUNT = 150;

// A regular zombie costs two small sun units in I, Zombie mode.
constexpr int REGULAR_ZOMBIE_SUN_COST = 50;

// Computes the red line x-coordinate from exact grass bounds so the initial
// line can sit precisely between the second and third columns.
int GetRedLineX(int stageStartCol) {
  return LAWN_GRID_LEFT + (stageStartCol + ZOMBIE_DEPLOYMENT_BUFFER_COLS) *
                              (LAWN_GRID_RIGHT - LAWN_GRID_LEFT) / GAME_COLS;
}

// The initial red line uses the first stage, not the final stage.
const int INITIAL_RED_LINE_X = GetRedLineX(INITIAL_ZOMBIE_DEPLOYMENT_START_COL);

// Returns whether a click is inside the lawn's rectangular grid bounds.
bool IsInsideLawnGrid(int x, int y) {
  return x >= LAWN_GRID_LEFT && x < LAWN_GRID_RIGHT && y >= LAWN_GRID_BOTTOM &&
         y < LAWN_GRID_TOP;
}

// Converts a click x-coordinate to the containing grid column.
int GetGridColFromX(int x) { return (x - LAWN_GRID_LEFT) / LAWN_GRID_WIDTH; }

// Converts a click y-coordinate to the top-to-bottom grid row.
int GetGridRowFromY(int y) { return (LAWN_GRID_TOP - y) / LAWN_GRID_HEIGHT; }

}  // namespace

// Initializes the object container and creates the static base interface.
void GameWorld::Init() {
  m_objects.clear();
  m_sunAmount = INITIAL_SUN_AMOUNT;
  m_sunCounterText.reset();
  m_selectedZombieCard = nullptr;
  m_cancelledZombieCardThisMouseDown = nullptr;
  InitStaticInterface();
}

// Advances one frame, then clears objects that died during updates.
LevelStatus GameWorld::Update() {
  const std::size_t objectsToUpdate = m_objects.size();
  auto iter = m_objects.begin();
  for (std::size_t updated = 0;
       updated < objectsToUpdate && iter != m_objects.end(); ++updated) {
    GameObjectPtr object = *iter;
    ++iter;
    if (object && object->IsAlive()) {
      object->Update();
    }
  }

  RemoveDeadObjects();
  return LevelStatus::ONGOING;
}

// Clears ownership so ObjectBase and TextBase unregister renderable elements.
void GameWorld::CleanUp() {
  m_objects.clear();
  m_sunAmount = 0;
  m_sunCounterText.reset();
  m_selectedZombieCard = nullptr;
  m_cancelledZombieCardThisMouseDown = nullptr;
}

// Creates all non-interactive assets required by the base interface.
void GameWorld::InitStaticInterface() {
  // Background is a world object so it participates in the normal render layer.
  AddObject(std::make_shared<BackgroundObject>());

  // Static top-bar UI elements show available sun, one card, and level
  // progress.
  m_sunCounterText = std::make_shared<SunCounterText>(m_sunAmount);
  AddObject(std::make_shared<ZombieCardObject>(
      ImageID::ZOMBIE_CARD_REGULAR, ZOMBIE_CARD_FIRST_X, ZOMBIE_CARD_Y));
  AddObject(
      std::make_shared<ProgressMeterObject>(ImageID::PROGRESS_METER_STAGE_1));

  // Red line and brains visualize the current deployment boundary and goals.
  AddObject(std::make_shared<RedLineObject>(INITIAL_RED_LINE_X));
  for (int row = 0; row < GAME_ROWS; ++row) {
    AddObject(std::make_shared<BrainObject>(row));
  }
}

// Null objects are ignored; valid objects receive a weak owner pointer.
void GameWorld::AddObject(GameObjectPtr object) {
  if (!object) {
    return;
  }
  object->SetWorld(weak_from_this());
  m_objects.push_back(std::move(object));
}

// Dead or null objects leave the container at frame end.
void GameWorld::RemoveDeadObjects() {
  if (m_selectedZombieCard && !m_selectedZombieCard->IsAlive()) {
    m_selectedZombieCard = nullptr;
  }
  m_objects.remove_if([](const GameObjectPtr& object) {
    return !object || !object->IsAlive();
  });
}

// Mutable visitor supports later interaction and collision systems.
void GameWorld::ForEachObject(const std::function<void(GameObject&)>& visitor) {
  for (const GameObjectPtr& object : m_objects) {
    if (object) {
      visitor(*object);
    }
  }
}

// Const visitor supports read-only queries and tests.
void GameWorld::ForEachObject(
    const std::function<void(const GameObject&)>& visitor) const {
  for (const GameObjectPtr& object : m_objects) {
    if (object) {
      visitor(*object);
    }
  }
}

// Every click first tries selected-card placement, then clears old selection.
void GameWorld::BeginMouseDown(int x, int y) {
  m_cancelledZombieCardThisMouseDown = m_selectedZombieCard;
  if (TryPlaceSelectedZombie(x, y)) {
    m_cancelledZombieCardThisMouseDown = nullptr;
  }
  ClearSelectedZombieCard();
}

// Selected regular-zombie cards place one zombie in the deployable lawn cells.
bool GameWorld::TryPlaceSelectedZombie(int x, int y) {
  if (!m_selectedZombieCard || !IsInsideLawnGrid(x, y) ||
      x < INITIAL_RED_LINE_X) {
    return false;
  }

  const int row = GetGridRowFromY(y);
  const int col = GetGridColFromX(x);
  if (!TrySpendSun(REGULAR_ZOMBIE_SUN_COST)) {
    return false;
  }
  AddObject(std::make_shared<RegularZombieObject>(row, col));
  m_selectedZombieCard->StartCooldown();
  return true;
}

// Spending sun updates both gameplay state and the visible counter atomically.
bool GameWorld::TrySpendSun(int sunCost) {
  if (m_sunAmount < sunCost) {
    return false;
  }
  m_sunAmount -= sunCost;
  if (m_sunCounterText) {
    m_sunCounterText->SetSunAmount(m_sunAmount);
  }
  return true;
}

// Selecting the just-cancelled card means toggling it off; other cards select.
void GameWorld::SelectZombieCard(ZombieCardObject& card) {
  if (m_cancelledZombieCardThisMouseDown == &card) {
    return;
  }
  if (card.IsCoolingDown()) {
    return;
  }
  card.SetSelected(true);
  m_selectedZombieCard = &card;
}

// Clearing selection restores the card to its normal slot when it still exists.
void GameWorld::ClearSelectedZombieCard() {
  if (m_selectedZombieCard) {
    m_selectedZombieCard->SetSelected(false);
  }
  m_selectedZombieCard = nullptr;
}

// Object count is exposed for simple validation and later UI/debug logic.
std::size_t GameWorld::GetObjectCount() const { return m_objects.size(); }
