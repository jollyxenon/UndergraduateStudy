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

  // Applies damage through the common zombie health pipeline.
  void TakeDamage(int damage) override;

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

// Draws a tougher bucket-head zombie that loses its bucket when weakened.
class BucketHeadZombieObject final : public ZombieObject {
 public:
  // Creates a bucket-head zombie centered on the requested grid cell.
  BucketHeadZombieObject(int row, int col);

  // Applies damage and refreshes the visible health stage immediately.
  void TakeDamage(int damage) override;

 private:
  // Switches to the regular zombie sprites after the bucket armor breaks.
  void RefreshHealthStageImage();

  // Records whether the bucket armor sprite has already been removed.
  bool m_bucketBroken = false;
};

#endif  // !ZOMBIEOBJECTS_HPP__
