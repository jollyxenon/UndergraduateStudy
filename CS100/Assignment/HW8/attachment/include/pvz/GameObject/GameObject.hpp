#ifndef GAMEOBJECT_HPP__
#define GAMEOBJECT_HPP__

#include <memory>

#include "pvz/Framework/ObjectBase.hpp"

// Declares the class name GameWorld so that its pointers can be used.
class GameWorld;

// Represents broad gameplay categories used by polymorphic queries.
enum class GameObjectType {
  GENERAL,
  PLANT,
  ZOMBIE,
  PROJECTILE,
  UI,
  SUN,
};

// Provides shared gameplay state and hooks for all world-owned objects.
class GameObject : public ObjectBase {
 public:
  // Creates a renderable gameplay object with optional health and grid
  // metadata.
  GameObject(ImageID imageID, int x, int y, LayerID layer, int width,
             int height, AnimID animID, int hp = 1, int row = -1, int col = -1,
             std::weak_ptr<GameWorld> world = {});

  // Destroys the gameplay object and lets ObjectBase unregister rendering
  // state.
  ~GameObject() override = default;

  // Updates one frame; subclasses override this for behavior.
  void Update() override;

  // Handles clicks; subclasses override this for interactive objects.
  void OnClick() override;

  // Returns the broad object category without checking image IDs.
  virtual GameObjectType GetType() const = 0;

  // Returns whether the object should remain in GameWorld's container.
  bool IsAlive() const;

  // Immediately marks the object for removal at the end of the frame.
  void Kill();

  // Returns current hit points.
  int GetHp() const;

  // Replaces hit points and kills the object when the value is non-positive.
  void SetHp(int hp);

  // Applies damage and kills the object if hit points reach zero.
  void TakeDamage(int damage);

  // Restores hit points for living objects.
  void Heal(int amount);

  // Returns grid row, or -1 if the object is not on the lawn grid.
  int GetRow() const;

  // Returns grid column, or -1 if the object is not on the lawn grid.
  int GetCol() const;

  // Updates cached grid position metadata.
  void SetGridPosition(int row, int col);

  // Stores the world that owns this object.
  void SetWorld(std::weak_ptr<GameWorld> world);

  // Returns the owning world when it still exists.
  std::shared_ptr<GameWorld> GetWorld() const;

 private:
  // Current hit points; non-positive values mean the object should be removed.
  int m_hp;

  // Whether the object is still active in the world container.
  bool m_alive;

  // Lawn row metadata, or -1 when the object is not grid-bound.
  int m_row;

  // Lawn column metadata, or -1 when the object is not grid-bound.
  int m_col;

  // Weak owner pointer prevents ownership cycles with GameWorld.
  std::weak_ptr<GameWorld> m_world;
};

#endif  // !GAMEOBJECT_HPP__
