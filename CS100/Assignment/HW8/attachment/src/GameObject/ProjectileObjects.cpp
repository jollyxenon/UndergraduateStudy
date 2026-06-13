#include "pvz/GameObject/ProjectileObjects.hpp"

#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Pea sprite dimensions match SpriteManager's registered pea texture.
constexpr int PEA_WIDTH = 28;
constexpr int PEA_HEIGHT = 28;
constexpr int PEA_HP = 1;
constexpr int PEA_MOVE_SPEED = 8;
constexpr int PEA_DAMAGE = 24;

}  // namespace

// Peas render above plants and travel horizontally on a lawn row.
PeaObject::PeaObject(int row, int x, int y)
    : GameObject(ImageID::PEA, x, y, LayerID::PROJECTILES, PEA_WIDTH,
                 PEA_HEIGHT, AnimID::NO_ANIMATION, PEA_HP, row, -1) {}

// Polymorphic type queries can identify projectiles without checking image IDs.
GameObjectType PeaObject::GetType() const { return GameObjectType::PROJECTILE; }

// A pea damages one zombie, otherwise moves right until it leaves the window.
void PeaObject::Update() {
  GameObject* zombie = GetWorld().FindCollidingZombie(*this);
  if (zombie) {
    zombie->TakeDamage(PEA_DAMAGE);
    Kill();
    return;
  }

  MoveTo(GetX() + PEA_MOVE_SPEED, GetY());
  if (GetX() - GetWidth() / 2 >= WINDOW_WIDTH) {
    Kill();
  }
}
