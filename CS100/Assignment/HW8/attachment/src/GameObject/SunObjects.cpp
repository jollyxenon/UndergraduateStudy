#include "pvz/GameObject/SunObjects.hpp"

#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Sun sprite dimensions match SpriteManager's registered sun animation frames.
constexpr int DROPPED_SUN_WIDTH = 80;
constexpr int DROPPED_SUN_HEIGHT = 80;
constexpr int DROPPED_SUN_HP = 1;

}  // namespace

// Dropped suns animate on the sun layer until clicked or cleared.
DroppedSunObject::DroppedSunObject(int x, int y)
    : GameObject(ImageID::SUN, x, y, LayerID::SUN, DROPPED_SUN_WIDTH,
                 DROPPED_SUN_HEIGHT, AnimID::IDLE, DROPPED_SUN_HP) {}

// Polymorphic type queries can identify suns without checking image IDs.
GameObjectType DroppedSunObject::GetType() const { return GameObjectType::SUN; }

// Clicking a sun collects its value exactly once and removes the sprite.
void DroppedSunObject::OnClick() {
  if (!IsAlive()) {
    return;
  }

  const std::shared_ptr<GameWorld> world = GetWorld();
  if (world) {
    world->AddSun(SUN_VALUE);
  }
  Kill();
}
