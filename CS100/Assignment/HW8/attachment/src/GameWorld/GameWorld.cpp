#include "pvz/GameWorld/GameWorld.hpp"

// Initializes the object container for a new level.
void GameWorld::Init() { m_objects.clear(); }

// Advances one frame, then clears objects that died during updates.
LevelStatus GameWorld::Update() {
  const std::size_t objectsToUpdate = m_objects.size();
  auto iter = m_objects.begin();
  for (std::size_t updated = 0;
       updated < objectsToUpdate && iter != m_objects.end(); ++updated) {
    GameObjectPtr object = *iter;
    ++iter;
    if (object && object->IsAlive()) {
      object->Update();
    }
  }

  RemoveDeadObjects();
  return LevelStatus::ONGOING;
}

// Clears ownership so ObjectBase unregisters renderable objects.
void GameWorld::CleanUp() { m_objects.clear(); }

// Null objects are ignored; valid objects receive a weak owner pointer.
void GameWorld::AddObject(GameObjectPtr object) {
  if (!object) {
    return;
  }
  object->SetWorld(weak_from_this());
  m_objects.push_back(std::move(object));
}

// Dead or null objects leave the container at frame end.
void GameWorld::RemoveDeadObjects() {
  m_objects.remove_if([](const GameObjectPtr& object) {
    return !object || !object->IsAlive();
  });
}

// Mutable visitor supports later interaction and collision systems.
void GameWorld::ForEachObject(const std::function<void(GameObject&)>& visitor) {
  for (const GameObjectPtr& object : m_objects) {
    if (object) {
      visitor(*object);
    }
  }
}

// Const visitor supports read-only queries and tests.
void GameWorld::ForEachObject(
    const std::function<void(const GameObject&)>& visitor) const {
  for (const GameObjectPtr& object : m_objects) {
    if (object) {
      visitor(*object);
    }
  }
}

// Object count is exposed for simple validation and later UI/debug logic.
std::size_t GameWorld::GetObjectCount() const { return m_objects.size(); }
