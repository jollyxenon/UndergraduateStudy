#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <array>
#include <functional>
#include <list>
#include <memory>

#include "pvz/Framework/TextBase.hpp"
#include "pvz/Framework/WorldBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/utils.hpp"

class SunCounterText;
class ZombieCardObject;
class PlantObject;
class ProgressMeterObject;
class RedLineObject;

// Owns and updates all gameplay objects for the current level.
class GameWorld : public WorldBase,
                  public std::enable_shared_from_this<GameWorld> {
 public:
  // Shared pointer type used for world-owned gameplay objects.
  using GameObjectPtr = std::shared_ptr<GameObject>;

  // Ordered container preserving stable iterators during frame updates.
  using GameObjectList = std::list<GameObjectPtr>;

  // Creates an empty game world; Init fills per-level state.
  GameWorld() = default;

  // Destroys the world and releases all owned objects.
  ~GameWorld() = default;

  // Prepares the world before the first frame.
  void Init() override;

  // Updates all owned game objects and returns current level state.
  LevelStatus Update() override;

  // Removes all owned objects and resets world state.
  void CleanUp() override;

  // Adds an object to world ownership and records its owning world.
  void AddObject(GameObjectPtr object);

  // Removes objects that have been marked dead.
  void RemoveDeadObjects();

  // Visits every currently owned object.
  void ForEachObject(const std::function<void(GameObject&)>& visitor);

  // Visits every currently owned object without allowing mutation.
  void ForEachObject(
      const std::function<void(const GameObject&)>& visitor) const;

  // Returns the first living plant touching the given zombie's body.
  PlantObject* FindCollidingPlant(const GameObject& zombie);

  // Returns the first living brain touching the given zombie's body.
  GameObject* FindCollidingBrain(const GameObject& zombie);

  // Returns whether a living zombie is on the same row and right side.
  bool HasZombieOnRight(int row, int x) const;

  // Returns the first living zombie touching the given projectile's body.
  GameObject* FindCollidingZombie(const GameObject& projectile);

  // Adds collected sun and refreshes the visible counter.
  void AddSun(int sunAmount);

  // Starts a mouse click by cancelling the previous card selection.
  void BeginMouseDown(int x, int y) override;

  // Makes the given zombie card the only selected card.
  void SelectZombieCard(ZombieCardObject& card);

  // Clears any selected zombie card.
  void ClearSelectedZombieCard();

  // Returns number of objects currently owned by the world.
  std::size_t GetObjectCount() const;

 private:
  // Creates static background and UI elements for the base interface.
  void InitStaticInterface();

  // Clears the per-cell plant occupancy records for a fresh level stage.
  void ClearPlantGrid();

  // Fills the red line's left side with random playable plant defenses.
  void GeneratePlantDefense();

  // Returns the number of plant columns available before the current red line.
  int GetPlantDefenseCols() const;

  // Returns the current red line x-coordinate for deployment checks.
  int GetCurrentRedLineX() const;

  // Returns whether any living brain remains in the current stage.
  bool HasLivingBrain() const;

  // Returns whether the player cannot deploy zombies while brains remain.
  bool IsFailedWithoutDeployableZombie() const;

  // Moves into the next stage after all brains have been eaten.
  LevelStatus AdvanceStage();

  // Resets dynamic stage state before rebuilding the next defense layout.
  void ResetStageState();

  // Restores spendable sun and refreshes the visible counter.
  void ResetSunAmount();

  // Recreates one brain target in every lawn row for a fresh stage.
  void RegenerateBrains();

  // Updates the progress meter sprite to match the current stage.
  void UpdateProgressMeter();

  // Attempts to place the previously selected zombie at the clicked grid cell.
  bool TryPlaceSelectedZombie(int x, int y);

  // Spends sun if enough is available for the requested zombie cost.
  bool TrySpendSun(int sunCost);

  // Clears the zombie card's selection and cooldown state for a fresh stage.
  void ResetZombieCards();

  // All gameplay objects currently owned by the world.
  GameObjectList m_objects;

  // Plant occupancy records prevent duplicate plants in one lawn cell.
  std::array<std::array<bool, GAME_COLS>, GAME_ROWS> m_plantGrid{};

  // Current spendable sun amount for zombie deployment.
  int m_sunAmount = 0;

  // Text object for the visible sun counter; TextBase self-registers globally.
  std::shared_ptr<SunCounterText> m_sunCounterText;

  // Red line object moved when a new stage starts.
  std::shared_ptr<RedLineObject> m_redLineObject;

  // Progress meter object updated when a new stage starts.
  std::shared_ptr<ProgressMeterObject> m_progressMeterObject;

  // Reusable zombie card object whose cooldown resets between stages.
  std::shared_ptr<ZombieCardObject> m_regularZombieCardObject;

  // Leftmost column where zombies may be deployed in the current stage.
  int m_currentZombieDeploymentStartCol = INITIAL_ZOMBIE_DEPLOYMENT_START_COL;

  // Currently selected zombie card; owned by m_objects and cleared on cleanup.
  ZombieCardObject* m_selectedZombieCard = nullptr;

  // Card cancelled at the start of this click, used to avoid reselecting it.
  ZombieCardObject* m_cancelledZombieCardThisMouseDown = nullptr;
};

#endif  // !GAMEWORLD_HPP__
