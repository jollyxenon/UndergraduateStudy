#ifndef UIOBJECTS_HPP__
#define UIOBJECTS_HPP__

#include <memory>
#include <string>

#include "pvz/Framework/TextBase.hpp"
#include "pvz/GameObject/GameObject.hpp"

// Provides a non-interactive UI image object with semantic subclasses below.
class StaticUIObject : public GameObject {
 public:
  // Creates a static image on the requested layer using assets registered by
  // SpriteManager.
  StaticUIObject(ImageID imageID, int x, int y, LayerID layer, int width,
                 int height);

  // Static UI objects are categorized as UI for later polymorphic queries.
  GameObjectType GetType() const override;
};

// Draws the full-screen game background from assets/background.png.
class BackgroundObject final : public StaticUIObject {
 public:
  // Places the background at the center of the 800x600 game window.
  BackgroundObject();
};

// Draws the semi-transparent overlay for a card that is cooling down.
class CooldownMaskObject final : public StaticUIObject {
 public:
  // Places the cooldown mask directly over the card slot.
  CooldownMaskObject(int x, int y);

  // Resizes the mask according to the remaining cooldown ratio.
  void SetRemainingRatio(double remainingRatio);

 private:
  // Original card center used to keep the shrinking mask top-aligned.
  int m_cardCenterY;
};

// Draws one zombie card in the top UI bar.
class ZombieCardObject : public StaticUIObject {
 public:
  // Places the selected zombie card image at its card slot.
  ZombieCardObject(ImageID imageID, int x, int y);

  // Destroys zombie card UI through the polymorphic base pointer.
  ~ZombieCardObject() override = default;

  // Asks the world to make this the only selected zombie card.
  void OnClick() override;

  // Counts down cooldown frames and removes the visible mask when done.
  void Update() override;

  // Applies or removes the selected visual state.
  void SetSelected(bool selected);

  // Starts the post-placement cooldown and shows its mask.
  void StartCooldown();

  // Clears any active cooldown and removes its visible mask immediately.
  void ResetCooldown();

  // Returns whether this card is temporarily unavailable.
  bool IsCoolingDown() const;

  // Returns whether this card is currently selected.
  bool IsSelected() const;

  // Returns whether this card may deploy before the current red line.
  virtual bool CanPlaceBeforeRedLine() const;

  // Returns the sun amount required to place this card's zombie.
  virtual int GetSunCost() const = 0;

  // Creates this card's zombie at the requested grid cell.
  virtual std::shared_ptr<GameObject> CreateZombie(int row, int col) const = 0;

 private:
  // Whether this card has already entered the selected visual state.
  bool m_selected;

  // Remaining update ticks before the card can be selected again.
  int m_cooldownFrames;

  // World-owned overlay that visually blocks this card while cooling down.
  std::shared_ptr<CooldownMaskObject> m_cooldownMask;
};

// Card that deploys a regular zombie.
class RegularZombieCardObject final : public ZombieCardObject {
 public:
  // Places the regular zombie card at the requested slot.
  RegularZombieCardObject(int x, int y);

  // Returns the regular zombie deployment cost.
  int GetSunCost() const override;

  // Creates a regular zombie at the requested grid cell.
  std::shared_ptr<GameObject> CreateZombie(int row, int col) const override;
};

// Card that deploys a bucket-head zombie.
class BucketHeadZombieCardObject final : public ZombieCardObject {
 public:
  // Places the bucket-head zombie card at the requested slot.
  BucketHeadZombieCardObject(int x, int y);

  // Returns the bucket-head zombie deployment cost.
  int GetSunCost() const override;

  // Creates a bucket-head zombie at the requested grid cell.
  std::shared_ptr<GameObject> CreateZombie(int row, int col) const override;
};

// Card that deploys a bungee zombie onto any lawn grid cell.
class BungeeZombieCardObject final : public ZombieCardObject {
 public:
  // Places the bungee zombie card at the requested slot.
  BungeeZombieCardObject(int x, int y);

  // Allows bungee zombies to target plant-side cells before the red line.
  bool CanPlaceBeforeRedLine() const override;

  // Returns the bungee zombie deployment cost.
  int GetSunCost() const override;

  // Creates a bungee zombie at the requested grid cell.
  std::shared_ptr<GameObject> CreateZombie(int row, int col) const override;
};

// Draws the red vertical deployment boundary line.
class RedLineObject final : public StaticUIObject {
 public:
  // Places the red line at the left edge of the current deployment area.
  explicit RedLineObject(int x);
};

// Draws one brain icon that zombies need to protect or eat.
class BrainObject final : public StaticUIObject {
 public:
  // Places a brain on the bottom line of the given lawn row.
  explicit BrainObject(int row);

  // Brains are a distinct gameplay target even though they render as UI.
  GameObjectType GetType() const override;
};

// Draws one progress meter sprite in the upper-right UI area.
class ProgressMeterObject final : public StaticUIObject {
 public:
  // Places a progress meter sprite at the fixed progress-meter location.
  explicit ProgressMeterObject(ImageID imageID);
};

// Draws the current sun value using the framework text renderer.
class SunCounterText final : public TextBase {
 public:
  // Creates a text counter at the standard sun counter position.
  explicit SunCounterText(int sunAmount);

  // Replaces the visible sun amount text.
  void SetSunAmount(int sunAmount);
};

#endif  // !UIOBJECTS_HPP__
