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

// Drops from above, grabs one target cell, then exits upward without walking.
class BungeeZombieObject final : public ZombieObject {
 public:
  // Creates a bungee zombie targeting the requested grid cell.
  BungeeZombieObject(int row, int col);

  // Advances the bungee-specific drop, grab, and retreat phases.
  void Update() override;

 private:
  // Tracks which part of the bungee attack is currently playing.
  enum class BungeeState { Descending, Grabbing, Ascending };

  // Removes the plant in the target cell when the grab lands.
  void GrabTargetAtCell();

  // Current bungee attack phase.
  BungeeState m_state = BungeeState::Descending;

  // Center y-coordinate of the target lawn cell.
  int m_targetY;

  // Remaining frames for the grab animation pause.
  int m_grabFramesRemaining = 0;
};

#endif  // !ZOMBIEOBJECTS_HPP__
