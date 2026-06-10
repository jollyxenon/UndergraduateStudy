#include "pvz/GameWorld/GameWorld.hpp"

#include "pvz/GameObject/UIObjects.hpp"

namespace {

// Base interface starts with one regular zombie card and this sun amount.
constexpr int INITIAL_SUN_AMOUNT = 150;

// Computes the red line x-coordinate from the current stage and its one-column
// deployment buffer.
int GetRedLineX(int stageStartCol) {
  return FIRST_COL_CENTER +
         (stageStartCol + ZOMBIE_DEPLOYMENT_BUFFER_COLS) * LAWN_GRID_WIDTH;
}

// The initial red line uses the first stage, not the final stage.
const int INITIAL_RED_LINE_X = GetRedLineX(INITIAL_ZOMBIE_DEPLOYMENT_START_COL);

}  // namespace

// Initializes the object container and creates the static base interface.
void GameWorld::Init() {
  m_objects.clear();
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
  m_sunCounterText = std::make_shared<SunCounterText>(INITIAL_SUN_AMOUNT);
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

// Every click first cancels the old card selection before dispatching targets.
void GameWorld::BeginMouseDown(int, int) {
  m_cancelledZombieCardThisMouseDown = m_selectedZombieCard;
  ClearSelectedZombieCard();
}

// Selecting the just-cancelled card means toggling it off; other cards select.
void GameWorld::SelectZombieCard(ZombieCardObject& card) {
  if (m_cancelledZombieCardThisMouseDown == &card) {
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
