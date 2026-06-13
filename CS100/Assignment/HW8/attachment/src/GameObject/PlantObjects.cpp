#include "pvz/GameObject/PlantObjects.hpp"

#include <array>
#include <utility>

#include "pvz/GameObject/ProjectileObjects.hpp"
#include "pvz/GameObject/SunObjects.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Assignment-specified plant sprite dimensions and hit points.
constexpr int PLANT_WIDTH = 60;
constexpr int PLANT_HEIGHT = 80;
constexpr int PLANT_HP = 340;
constexpr int PEASHOOTER_FIRE_INTERVAL_FRAMES = 32;
constexpr int PEA_SPAWN_X_OFFSET = 30;
constexpr int SUNFLOWER_DEATH_SUN_COUNT = 6;

// Offsets spread dropped suns around the defeated sunflower for clicking.
const std::array<std::pair<int, int>, SUNFLOWER_DEATH_SUN_COUNT>
    SUNFLOWER_DEATH_SUN_OFFSETS{
        {{-30, 24}, {0, 30}, {30, 24}, {-24, -18}, {0, -28}, {24, -18}}};

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

// Plants can run subclass-specific cleanup before the shared kill logic.
void PlantObject::Kill() {
  m_deathHandled = true;
  OnDeath();
  GameObject::Kill();
}

// Some effects remove a plant without triggering its death animation or drops.
void PlantObject::KillWithoutDeathEffect() {
  m_deathHandled = true;
  GameObject::Kill();
}

// Most plants do not need death-side effects.
void PlantObject::OnDeath() {}

// Base plants have no per-frame behavior by default.
void PlantObject::Update() {}

// Sunflowers start in the idle animation at the target grid center.
SunflowerObject::SunflowerObject(int row, int col)
    : PlantObject(ImageID::SUNFLOWER, GetGridCenterX(col), GetGridCenterY(row),
                  PLANT_HP, row, col) {}

// A defeated sunflower bursts into collectible suns at nearby positions.
void SunflowerObject::OnDeath() {
  for (const auto& [xOffset, yOffset] : SUNFLOWER_DEATH_SUN_OFFSETS) {
    GetWorld().AddObject(
        std::make_shared<DroppedSunObject>(GetX() + xOffset, GetY() + yOffset));
  }
}

// Peashooters start in the idle animation at the target grid center.
PeashooterObject::PeashooterObject(int row, int col)
    : PlantObject(ImageID::PEASHOOTER, GetGridCenterX(col), GetGridCenterY(row),
                  PLANT_HP, row, col) {}

// Peashooters fire at a steady pace while a same-row zombie is to the right.
void PeashooterObject::Update() {
  PlantObject::Update();
  if (m_shootCooldown > 0) {
    --m_shootCooldown;
  }

  if (!GetWorld().HasZombieOnRight(GetRow(), GetX()) || m_shootCooldown > 0) {
    return;
  }

  GetWorld().AddObject(std::make_shared<PeaObject>(
      GetRow(), GetX() + PEA_SPAWN_X_OFFSET, GetY() + PLANT_HEIGHT / 8));
  m_shootCooldown = PEASHOOTER_FIRE_INTERVAL_FRAMES;
}
