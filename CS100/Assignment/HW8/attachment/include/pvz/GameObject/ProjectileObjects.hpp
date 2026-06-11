#ifndef PROJECTILEOBJECTS_HPP__
#define PROJECTILEOBJECTS_HPP__

#include "pvz/GameObject/GameObject.hpp"

// Provides shared behavior for moving plant projectiles.
class PeaObject final : public GameObject {
 public:
  // Creates a pea projectile on the requested row at the requested position.
  PeaObject(int row, int x, int y);

  // Projectiles are grouped under the projectile category.
  GameObjectType GetType() const override;

  // Moves right and damages the first same-row zombie it touches.
  void Update() override;
};

#endif  // !PROJECTILEOBJECTS_HPP__
