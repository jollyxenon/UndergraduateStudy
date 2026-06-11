#ifndef PLANTOBJECTS_HPP__
#define PLANTOBJECTS_HPP__

#include "pvz/GameObject/GameObject.hpp"

// Provides shared rendering, HP, and grid metadata for all plant objects.
class PlantObject : public GameObject {
 public:
  // Creates a plant at the given grid cell and render position.
  PlantObject(ImageID imageID, int x, int y, int hp, int row, int col);

  // Plants are grouped under the plant category.
  GameObjectType GetType() const override;

  // Dead plants have no per-frame behavior before GameWorld removes them.
  void Update() override;
};

// Draws an I, Zombie sunflower occupying one lawn grid cell.
class SunflowerObject final : public PlantObject {
 public:
  // Creates a sunflower centered on the requested grid cell.
  SunflowerObject(int row, int col);
};

// Draws an I, Zombie peashooter occupying one lawn grid cell.
class PeashooterObject final : public PlantObject {
 public:
  // Creates a peashooter centered on the requested grid cell.
  PeashooterObject(int row, int col);

  // Fires peas only when a living zombie exists to the right on the same row.
  void Update() override;

 private:
  // Tracks frames remaining until this peashooter can fire again.
  int m_shootCooldown = 0;
};

#endif  // !PLANTOBJECTS_HPP__
