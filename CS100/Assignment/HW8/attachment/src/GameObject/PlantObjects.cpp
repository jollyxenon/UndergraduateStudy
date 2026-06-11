#include "pvz/GameObject/PlantObjects.hpp"

namespace {

// Assignment-specified plant sprite dimensions and hit points.
constexpr int PLANT_WIDTH = 60;
constexpr int PLANT_HEIGHT = 80;
constexpr int PLANT_HP = 340;

// Converts a grid column index to the center x-coordinate of that cell.
int GetGridCenterX(int col) {
  return LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2;
}

// Converts a top-to-bottom grid row index to the center y-coordinate.
int GetGridCenterY(int row) {
  return LAWN_GRID_TOP - row * LAWN_GRID_HEIGHT - LAWN_GRID_HEIGHT / 2;
}

}  // namespace

// Plants render on the plant layer and idle by default.
PlantObject::PlantObject(ImageID imageID, int x, int y, int hp, int row,
                         int col)
    : GameObject(imageID, x, y, LayerID::PLANTS, PLANT_WIDTH, PLANT_HEIGHT,
                 AnimID::IDLE, hp, row, col) {}

// Polymorphic type queries can identify plants without checking image IDs.
GameObjectType PlantObject::GetType() const { return GameObjectType::PLANT; }

// Plant actions are skipped once the object has been killed.
void PlantObject::Update() {
  if (!IsAlive()) {
    return;
  }
}

// Sunflowers start in the idle animation at the target grid center.
SunflowerObject::SunflowerObject(int row, int col)
    : PlantObject(ImageID::SUNFLOWER, GetGridCenterX(col), GetGridCenterY(row),
                  PLANT_HP, row, col) {}

// Peashooters start in the idle animation at the target grid center.
PeashooterObject::PeashooterObject(int row, int col)
    : PlantObject(ImageID::PEASHOOTER, GetGridCenterX(col), GetGridCenterY(row),
                  PLANT_HP, row, col) {}
