#include "pvz/GameObject/ZombieObjects.hpp"

namespace {

// Regular zombie sprite dimensions match SpriteManager's registered walk
// frames.
constexpr int REGULAR_ZOMBIE_WIDTH = 100;
constexpr int REGULAR_ZOMBIE_HEIGHT = 139;
constexpr int REGULAR_ZOMBIE_HP = 270;

// Converts a grid column index to the center x-coordinate of that cell.
int GetGridCenterX(int col) {
  return LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2;
}

// Converts a top-to-bottom grid row index to the center y-coordinate.
int GetGridCenterY(int row) {
  return LAWN_GRID_TOP - row * LAWN_GRID_HEIGHT - LAWN_GRID_HEIGHT / 2;
}

}  // namespace

// Zombies render on the zombie layer and walk by default.
ZombieObject::ZombieObject(ImageID imageID, int x, int y, int width, int height,
                           int hp, int row, int col)
    : GameObject(imageID, x, y, LayerID::ZOMBIES, width, height, AnimID::WALK,
                 hp, row, col) {}

// Polymorphic type queries can identify zombies without checking image IDs.
GameObjectType ZombieObject::GetType() const { return GameObjectType::ZOMBIE; }

// Regular zombies start in the walking animation at the target grid center.
RegularZombieObject::RegularZombieObject(int row, int col)
    : ZombieObject(ImageID::REGULAR_ZOMBIE, GetGridCenterX(col),
                   GetGridCenterY(row), REGULAR_ZOMBIE_WIDTH,
                   REGULAR_ZOMBIE_HEIGHT, REGULAR_ZOMBIE_HP, row, col) {}
