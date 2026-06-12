#include "pvz/GameObject/GameObject.hpp"

GameObject::GameObject(ImageID imageID, int x, int y, LayerID layer, int width,
                       int height, AnimID animID, int hp, int row, int col,
                       std::weak_ptr<GameWorld> world)
    : ObjectBase(imageID, x, y, layer, width, height, animID),
      m_hp(hp),
      m_alive(hp > 0),
      m_row(row),
      m_col(col),
      m_world(std::move(world)) {}

// Base objects have no per-frame behavior by default.
void GameObject::Update() {}

// Base objects do not react to clicks by default.
void GameObject::OnClick() {}

// The world container keeps only living objects.
bool GameObject::IsAlive() const { return m_alive && m_hp > 0; }

// Dead objects are erased by GameWorld after updates finish.
void GameObject::Kill() {
  if (!m_alive) {
    return;
  }
  m_alive = false;
  m_hp = 0;
}

// Hit points are stored by the shared game-object base.
int GameObject::GetHp() const { return m_hp; }

// Non-positive hit points mean the object is dead.
void GameObject::SetHp(int hp) {
  m_hp = hp;
  if (m_hp <= 0) {
    Kill();
  } else {
    m_alive = true;
  }
}

// Damage ignores non-positive values to avoid accidental healing.
void GameObject::TakeDamage(int damage) {
  if (damage <= 0 || !IsAlive()) {
    return;
  }
  SetHp(m_hp - damage);
}

// Row metadata lets subclasses query grid location without recomputing it.
int GameObject::GetRow() const { return m_row; }

// Column metadata lets subclasses query grid location without recomputing it.
int GameObject::GetCol() const { return m_col; }

// Grid metadata is optional for objects that are not placed on the lawn.
void GameObject::SetGridPosition(int row, int col) {
  m_row = row;
  m_col = col;
}

// The world pointer is weak so objects do not keep GameWorld alive.
void GameObject::SetWorld(std::weak_ptr<GameWorld> world) {
  m_world = std::move(world);
}

// Game objects are updated only after GameWorld assigns their owner.
GameWorld& GameObject::GetWorld() const { return *m_world.lock(); }
