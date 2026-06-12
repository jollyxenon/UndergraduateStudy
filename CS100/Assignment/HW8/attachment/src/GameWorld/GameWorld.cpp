#include "pvz/GameWorld/GameWorld.hpp"

#include "pvz/GameObject/PlantObjects.hpp"
#include "pvz/GameObject/UIObjects.hpp"
#include "pvz/GameObject/ZombieObjects.hpp"

namespace {

// Base interface starts with one regular zombie card and this sun amount.
constexpr int INITIAL_SUN_AMOUNT = 150;

// Falling below this amount means no zombie card can be used anymore.
constexpr int MINIMUM_ZOMBIE_SUN_COST = 50;

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
  m_redLineObject.reset();
  m_progressMeterObject.reset();
  m_regularZombieCardObject.reset();
  m_bucketHeadZombieCardObject.reset();
  m_bungeeZombieCardObject.reset();
  m_currentZombieDeploymentStartCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL;
  m_selectedZombieCard = nullptr;
  m_cancelledZombieCardThisMouseDown = nullptr;
  ClearPlantGrid();
  InitStaticInterface();
  GeneratePlantDefense();
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
  if (IsFailedWithoutDeployableZombie()) {
    return LevelStatus::LOSING;
  }
  if (!HasLivingBrain()) {
    return AdvanceStage();
  }
  return LevelStatus::ONGOING;
}

// Clears ownership so ObjectBase and TextBase unregister renderable elements.
void GameWorld::CleanUp() {
  m_objects.clear();
  m_sunAmount = 0;
  m_sunCounterText.reset();
  m_redLineObject.reset();
  m_progressMeterObject.reset();
  m_regularZombieCardObject.reset();
  m_bucketHeadZombieCardObject.reset();
  m_bungeeZombieCardObject.reset();
  m_currentZombieDeploymentStartCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL;
  m_selectedZombieCard = nullptr;
  m_cancelledZombieCardThisMouseDown = nullptr;
  ClearPlantGrid();
}

// Creates all non-interactive assets required by the base interface.
void GameWorld::InitStaticInterface() {
  // Background is a world object so it participates in the normal render layer.
  AddObject(std::make_shared<BackgroundObject>());

  // Static top-bar UI elements show available sun, zombie cards, and level
  // progress.
  m_sunCounterText = std::make_shared<SunCounterText>(m_sunAmount);
  m_regularZombieCardObject = std::make_shared<RegularZombieCardObject>(
      ZOMBIE_CARD_FIRST_X, ZOMBIE_CARD_Y);
  AddObject(m_regularZombieCardObject);
  m_bucketHeadZombieCardObject = std::make_shared<BucketHeadZombieCardObject>(
      ZOMBIE_CARD_FIRST_X + ZOMBIE_CARD_SPACING, ZOMBIE_CARD_Y);
  AddObject(m_bucketHeadZombieCardObject);
  m_bungeeZombieCardObject = std::make_shared<BungeeZombieCardObject>(
      ZOMBIE_CARD_FIRST_X + 2 * ZOMBIE_CARD_SPACING, ZOMBIE_CARD_Y);
  AddObject(m_bungeeZombieCardObject);
  m_progressMeterObject =
      std::make_shared<ProgressMeterObject>(ImageID::PROGRESS_METER_STAGE_1);
  AddObject(m_progressMeterObject);

  // Red line and brains visualize the current deployment boundary and goals.
  m_redLineObject = std::make_shared<RedLineObject>(INITIAL_RED_LINE_X);
  AddObject(m_redLineObject);
  for (int row = 0; row < GAME_ROWS; ++row) {
    AddObject(std::make_shared<BrainObject>(row));
  }
}

// Plant occupancy is reset before generating a new level or stage defense.
void GameWorld::ClearPlantGrid() {
  for (auto& row : m_plantGrid) {
    row.fill(false);
  }
}

// Every lawn cell before the red line receives one random plant defense.
void GameWorld::GeneratePlantDefense() {
  const int plantDefenseCols = GetPlantDefenseCols();
  for (int row = 0; row < GAME_ROWS; ++row) {
    for (int col = 0; col < plantDefenseCols; ++col) {
      m_plantGrid[row][col] = true;
      if (randInt(0, 1) == 1) {
        AddObject(std::make_shared<PeashooterObject>(row, col));
      } else {
        AddObject(std::make_shared<SunflowerObject>(row, col));
      }
    }
  }
}

// Plant columns are all lawn cells to the left of the current deployment area.
int GameWorld::GetPlantDefenseCols() const {
  return m_currentZombieDeploymentStartCol + ZOMBIE_DEPLOYMENT_BUFFER_COLS;
}

// Deployment checks use the same dynamic boundary as the rendered red line.
int GameWorld::GetCurrentRedLineX() const {
  return GetRedLineX(m_currentZombieDeploymentStartCol);
}

// A stage is complete when every row's brain has been removed from the world.
bool GameWorld::HasLivingBrain() const {
  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        object->GetType() == GameObjectType::BRAIN) {
      return true;
    }
  }
  return false;
}

// Failure occurs when no active zombie can eat the remaining brains and the
// player cannot afford another regular zombie.
bool GameWorld::IsFailedWithoutDeployableZombie() const {
  if (m_sunAmount >= MINIMUM_ZOMBIE_SUN_COST || !HasLivingBrain()) {
    return false;
  }

  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        object->GetType() == GameObjectType::ZOMBIE) {
      return false;
    }
  }
  return true;
}

// Stage advancement moves the boundary right and rebuilds all dynamic targets.
LevelStatus GameWorld::AdvanceStage() {
  if (m_currentZombieDeploymentStartCol >= FINAL_ZOMBIE_DEPLOYMENT_START_COL) {
    return LevelStatus::WINNING;
  }

  ++m_currentZombieDeploymentStartCol;
  if (m_redLineObject && m_redLineObject->IsAlive()) {
    m_redLineObject->MoveTo(GetCurrentRedLineX(), m_redLineObject->GetY());
  }
  UpdateProgressMeter();

  ResetStageState();
  ClearPlantGrid();
  GeneratePlantDefense();
  RegenerateBrains();
  RemoveDeadObjects();
  return LevelStatus::ONGOING;
}

// Stage reset removes transient actors and restores reusable UI state.
void GameWorld::ResetStageState() {
  ClearSelectedZombieCard();
  m_cancelledZombieCardThisMouseDown = nullptr;
  ResetSunAmount();
  ResetZombieCards();

  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        (object->GetType() == GameObjectType::PLANT ||
         object->GetType() == GameObjectType::PROJECTILE ||
         object->GetType() == GameObjectType::SUN ||
         object->GetType() == GameObjectType::ZOMBIE ||
         object->GetType() == GameObjectType::BRAIN)) {
      object->Kill();
    }
  }
}

// Stage starts always grant the same deployment budget as the first stage.
void GameWorld::ResetSunAmount() {
  m_sunAmount = INITIAL_SUN_AMOUNT;
  if (m_sunCounterText) {
    m_sunCounterText->SetSunAmount(m_sunAmount);
  }
}

// Reusable card UI should not carry selection or cooldown into a new stage.
void GameWorld::ResetZombieCards() {
  if (m_regularZombieCardObject && m_regularZombieCardObject->IsAlive()) {
    m_regularZombieCardObject->ResetCooldown();
  }
  if (m_bucketHeadZombieCardObject && m_bucketHeadZombieCardObject->IsAlive()) {
    m_bucketHeadZombieCardObject->ResetCooldown();
  }
  if (m_bungeeZombieCardObject && m_bungeeZombieCardObject->IsAlive()) {
    m_bungeeZombieCardObject->ResetCooldown();
  }
}

// New stages restore the five row targets after the previous brains disappear.
void GameWorld::RegenerateBrains() {
  for (int row = 0; row < GAME_ROWS; ++row) {
    AddObject(std::make_shared<BrainObject>(row));
  }
}

// The meter stage images are consecutive enum values, so the stage offset maps
// directly to the current progress sprite.
void GameWorld::UpdateProgressMeter() {
  if (!m_progressMeterObject || !m_progressMeterObject->IsAlive()) {
    return;
  }

  const int stageIndex =
      m_currentZombieDeploymentStartCol - INITIAL_ZOMBIE_DEPLOYMENT_START_COL;
  m_progressMeterObject->ChangeImage(static_cast<ImageID>(
      static_cast<int>(ImageID::PROGRESS_METER_STAGE_1) + stageIndex));
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

// Plant collision uses same-row bounding boxes and category metadata.
PlantObject* GameWorld::FindCollidingPlant(const GameObject& zombie) {
  for (const GameObjectPtr& object : m_objects) {
    if (!object || !object->IsAlive() ||
        object->GetType() != GameObjectType::PLANT ||
        object->GetRow() != zombie.GetRow()) {
      continue;
    }

    const int zombieLeft = zombie.GetX() - zombie.GetWidth() / 2;
    const int zombieRight = zombie.GetX() + zombie.GetWidth() / 2;
    const int plantLeft = object->GetX() - object->GetWidth() / 2;
    const int plantRight = object->GetX() + object->GetWidth() / 2;
    if (zombieLeft <= plantRight && zombieRight >= plantLeft) {
      return static_cast<PlantObject*>(object.get());
    }
  }
  return nullptr;
}

// Grid lookup uses category metadata instead of concrete type or image checks.
PlantObject* GameWorld::FindPlantAt(int row, int col) {
  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        object->GetType() == GameObjectType::PLANT && object->GetRow() == row &&
        object->GetCol() == col) {
      return static_cast<PlantObject*>(object.get());
    }
  }
  return nullptr;
}

// Brain collision uses same-row bounding boxes and category metadata.
GameObject* GameWorld::FindCollidingBrain(const GameObject& zombie) {
  for (const GameObjectPtr& object : m_objects) {
    if (!object || !object->IsAlive() ||
        object->GetType() != GameObjectType::BRAIN ||
        object->GetRow() != zombie.GetRow()) {
      continue;
    }

    const int zombieLeft = zombie.GetX() - zombie.GetWidth() / 2;
    const int zombieRight = zombie.GetX() + zombie.GetWidth() / 2;
    const int brainLeft = object->GetX() - object->GetWidth() / 2;
    const int brainRight = object->GetX() + object->GetWidth() / 2;
    if (zombieLeft <= brainRight && zombieRight >= brainLeft) {
      return object.get();
    }
  }
  return nullptr;
}

// Grid lookup finds a living brain at the requested cell when one exists.
GameObject* GameWorld::FindBrainAt(int row, int col) {
  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        object->GetType() == GameObjectType::BRAIN && object->GetRow() == row &&
        object->GetCol() == col) {
      return object.get();
    }
  }
  return nullptr;
}

// Shooter targeting checks for any same-row zombie strictly to the right.
bool GameWorld::HasZombieOnRight(int row, int x) const {
  for (const GameObjectPtr& object : m_objects) {
    if (object && object->IsAlive() &&
        object->GetType() == GameObjectType::ZOMBIE &&
        object->GetRow() == row && object->GetX() > x) {
      return true;
    }
  }
  return false;
}

// Projectile collision uses same-row bounding boxes and category metadata.
GameObject* GameWorld::FindCollidingZombie(const GameObject& projectile) {
  GameObject* firstZombie = nullptr;
  for (const GameObjectPtr& object : m_objects) {
    if (!object || !object->IsAlive() ||
        object->GetType() != GameObjectType::ZOMBIE ||
        object->GetRow() != projectile.GetRow()) {
      continue;
    }

    const int projectileLeft = projectile.GetX() - projectile.GetWidth() / 2;
    const int projectileRight = projectile.GetX() + projectile.GetWidth() / 2;
    const int zombieLeft = object->GetX() - object->GetWidth() / 2;
    const int zombieRight = object->GetX() + object->GetWidth() / 2;
    if (projectileLeft <= zombieRight && projectileRight >= zombieLeft &&
        (!firstZombie || object->GetX() < firstZombie->GetX())) {
      firstZombie = object.get();
    }
  }
  return firstZombie;
}

// Every click first tries selected-card placement, then clears old selection.
void GameWorld::BeginMouseDown(int x, int y) {
  m_cancelledZombieCardThisMouseDown = m_selectedZombieCard;
  if (TryPlaceSelectedZombie(x, y)) {
    m_cancelledZombieCardThisMouseDown = nullptr;
  }
  ClearSelectedZombieCard();
}

// Selected zombie cards place one zombie when their placement rule allows it.
bool GameWorld::TryPlaceSelectedZombie(int x, int y) {
  if (!m_selectedZombieCard || !IsInsideLawnGrid(x, y)) {
    return false;
  }

  const int row = GetGridRowFromY(y);
  const int col = GetGridColFromX(x);
  if (x < GetCurrentRedLineX() &&
      !m_selectedZombieCard->CanPlaceBeforeRedLine()) {
    return false;
  }
  if (!TrySpendSun(m_selectedZombieCard->GetSunCost())) {
    return false;
  }
  AddObject(m_selectedZombieCard->CreateZombie(row, col));
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

// Collected suns update both gameplay state and visible text atomically.
void GameWorld::AddSun(int sunAmount) {
  if (sunAmount <= 0) {
    return;
  }
  m_sunAmount += sunAmount;
  if (m_sunCounterText) {
    m_sunCounterText->SetSunAmount(m_sunAmount);
  }
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
