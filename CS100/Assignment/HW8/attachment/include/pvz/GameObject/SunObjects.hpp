#ifndef SUNOBJECTS_HPP__
#define SUNOBJECTS_HPP__

#include "pvz/GameObject/GameObject.hpp"

// Represents one collectible sun dropped by a defeated sunflower.
class DroppedSunObject final : public GameObject {
 public:
  // Creates a sun at the requested screen position.
  DroppedSunObject(int x, int y);

  // Sun objects are grouped under the sun category.
  GameObjectType GetType() const override;

  // Collects this sun into the player's sun counter.
  void OnClick() override;
};

#endif  // !SUNOBJECTS_HPP__
