#ifndef ZOMBIEOBJECTS_HPP__
#define ZOMBIEOBJECTS_HPP__

#include "pvz/GameObject/GameObject.hpp"

// Provides shared behavior and metadata for all zombie objects.
class ZombieObject : public GameObject {
 public:
  // Creates a zombie at the given grid cell and render position.
  ZombieObject(ImageID imageID, int x, int y, int width, int height, int hp,
               int row, int col);

  // Zombies are grouped under the zombie category.
  GameObjectType GetType() const override;

  // Advances the walking zombie one frame toward the left side of the lawn.
  void Update() override;

 private:
  // Tracks frames remaining until this zombie can complete the next bite.
  int m_biteCooldown = 0;
};

// Draws a regular walking zombie placed by the zombie card.
class RegularZombieObject final : public ZombieObject {
 public:
  // Creates a regular zombie centered on the requested grid cell.
  RegularZombieObject(int row, int col);
};

#endif  // !ZOMBIEOBJECTS_HPP__
