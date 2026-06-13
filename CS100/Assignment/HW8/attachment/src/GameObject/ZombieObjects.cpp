#include "pvz/GameObject/ZombieObjects.hpp"

#include "pvz/GameObject/PlantObjects.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Regular zombie sprite dimensions match SpriteManager's registered walk
// frames.
constexpr int REGULAR_ZOMBIE_WIDTH = 100;
constexpr int REGULAR_ZOMBIE_HEIGHT = 139;
constexpr int REGULAR_ZOMBIE_HP = 260;
constexpr int BUCKET_HEAD_ZOMBIE_HP = 1450;
constexpr int BUNGEE_ZOMBIE_WIDTH = 102;
constexpr int BUNGEE_ZOMBIE_HEIGHT = 90;
constexpr int BUNGEE_ZOMBIE_HP = 450;
constexpr int ZOMBIE_WALK_SPEED = 1;
constexpr int ZOMBIE_BITE_DAMAGE = 4;
constexpr int ZOMBIE_BITE_INTERVAL_FRAMES = 1;
constexpr int BUNGEE_ZOMBIE_SPEED = 24;
constexpr int BUNGEE_ZOMBIE_GRAB_FRAMES = 30;

// Zombies die after their whole sprite leaves the left edge of the window.
constexpr int ZOMBIE_LEFT_EXIT_X = 0;

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

// Shared zombie damage currently delegates to the common GameObject HP logic.
void ZombieObject::TakeDamage(int damage) { GameObject::TakeDamage(damage); }

// Normal walking zombies are valid progress threats for failure detection.
bool ZombieObject::CanThreatenBrain() const { return true; }

// Zombies stop to bite colliding plants, otherwise they keep walking left.
void ZombieObject::Update() {
  PlantObject* plant = GetWorld().FindCollidingPlant(*this);
  if (plant) {
    if (GetCurrentAnimation() != AnimID::EAT) {
      PlayAnimation(AnimID::EAT);
    }
    if (m_biteCooldown <= 0) {
      plant->TakeDamage(ZOMBIE_BITE_DAMAGE);
      m_biteCooldown = ZOMBIE_BITE_INTERVAL_FRAMES;
    } else {
      --m_biteCooldown;
    }
    return;
  }

  GameObject* brain = GetWorld().FindCollidingBrain(*this);
  if (brain) {
    brain->Kill();
  }

  m_biteCooldown = 0;
  if (GetCurrentAnimation() != AnimID::WALK) {
    PlayAnimation(AnimID::WALK);
  }
  MoveTo(GetX() - ZOMBIE_WALK_SPEED, GetY());
  if (GetX() + GetWidth() / 2 < ZOMBIE_LEFT_EXIT_X) {
    Kill();
  }
}

// Regular zombies start in the walking animation at the target grid center.
RegularZombieObject::RegularZombieObject(int row, int col)
    : ZombieObject(ImageID::REGULAR_ZOMBIE, GetGridCenterX(col),
                   GetGridCenterY(row), REGULAR_ZOMBIE_WIDTH,
                   REGULAR_ZOMBIE_HEIGHT, REGULAR_ZOMBIE_HP, row, col) {}

// Bucket-head zombies reuse the regular zombie body size with higher HP.
BucketHeadZombieObject::BucketHeadZombieObject(int row, int col)
    : ZombieObject(ImageID::BUCKET_HEAD_ZOMBIE, GetGridCenterX(col),
                   GetGridCenterY(row), REGULAR_ZOMBIE_WIDTH,
                   REGULAR_ZOMBIE_HEIGHT, BUCKET_HEAD_ZOMBIE_HP, row, col) {}

// Damage can break the bucket armor before the zombie itself dies.
void BucketHeadZombieObject::TakeDamage(int damage) {
  ZombieObject::TakeDamage(damage);
  RefreshHealthStageImage();
}

// Once armor HP is gone, regular walk/eat animations represent the exposed
// body.
void BucketHeadZombieObject::RefreshHealthStageImage() {
  if (GetHp() > REGULAR_ZOMBIE_HP) {
    return;
  }
  m_bucketBroken = true;
  ChangeImage(ImageID::REGULAR_ZOMBIE);
}

// Bungee zombies start above the screen and keep their target cell metadata.
BungeeZombieObject::BungeeZombieObject(int row, int col)
    : ZombieObject(ImageID::BUNGEE_ZOMBIE, GetGridCenterX(col),
                   WINDOW_HEIGHT + BUNGEE_ZOMBIE_HEIGHT, BUNGEE_ZOMBIE_WIDTH,
                   BUNGEE_ZOMBIE_HEIGHT, BUNGEE_ZOMBIE_HP, row, col),
      m_state(BungeeState::Descending),
      m_targetY(GetGridCenterY(row)),
      m_grabFramesRemaining(0) {
  PlayAnimation(AnimID::JUMP);
}

// Bungee zombies do not use the shared walk-and-bite zombie behavior.
void BungeeZombieObject::Update() {
  if (m_state == BungeeState::Descending) {
    const int nextY = GetY() - BUNGEE_ZOMBIE_SPEED;
    if (nextY <= m_targetY) {
      MoveTo(GetX(), m_targetY);
      ChangeImage(ImageID::BUNGEE_ZOMBIE_GRAB);
      PlayAnimation(AnimID::JUMP);
      GrabTargetAtCell();
      m_state = BungeeState::Grabbing;
      m_grabFramesRemaining = BUNGEE_ZOMBIE_GRAB_FRAMES;
    } else {
      MoveTo(GetX(), nextY);
    }
    return;
  }

  if (m_state == BungeeState::Grabbing) {
    if (m_grabFramesRemaining > 0) {
      --m_grabFramesRemaining;
      return;
    }
    ChangeImage(ImageID::BUNGEE_ZOMBIE);
    PlayAnimation(AnimID::JUMP);
    m_state = BungeeState::Ascending;
  }

  MoveTo(GetX(), GetY() + BUNGEE_ZOMBIE_SPEED);
  if (GetY() - GetHeight() / 2 > WINDOW_HEIGHT) {
    Kill();
  }
}

// A bungee zombie exits upward after stealing a plant, so it cannot eat brains.
bool BungeeZombieObject::CanThreatenBrain() const { return false; }

// The landing grab only steals a plant from the target cell.
void BungeeZombieObject::GrabTargetAtCell() {
  PlantObject* plant = GetWorld().FindPlantAt(GetRow(), GetCol());
  plant->KillWithoutDeathEffect();
}
