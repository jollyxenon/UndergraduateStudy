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

  // Marks the plant dead after running plant-specific death behavior.
  void Kill() override;

  // Marks the plant dead without running plant-specific death behavior.
  void KillWithoutDeathEffect();

  // Dead plants have no per-frame behavior before GameWorld removes them.
  void Update() override;

 protected:
  // Allows plant subclasses to react when the plant is killed.
  virtual void OnDeath();

 private:
  // Prevents duplicate plant death behavior when Kill is called repeatedly.
  bool m_deathHandled = false;
};

// Draws an I, Zombie sunflower occupying one lawn grid cell.
class SunflowerObject final : public PlantObject {
 public:
  // Creates a sunflower centered on the requested grid cell.
  SunflowerObject(int row, int col);

 protected:
  // Drops collectible sun when the sunflower dies.
  void OnDeath() override;
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
