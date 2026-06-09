#ifndef GAMEWORLD_HPP__
#define GAMEWORLD_HPP__

#include <functional>
#include <list>
#include <memory>

#include "pvz/Framework/TextBase.hpp"
#include "pvz/Framework/WorldBase.hpp"
#include "pvz/GameObject/GameObject.hpp"
#include "pvz/utils.hpp"

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

  // Returns number of objects currently owned by the world.
  std::size_t GetObjectCount() const;

 private:
  // All gameplay objects currently owned by the world.
  GameObjectList m_objects;
};

#endif  // !GAMEWORLD_HPP__
