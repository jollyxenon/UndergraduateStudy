#ifndef UIOBJECTS_HPP__
#define UIOBJECTS_HPP__

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

// Draws one zombie card in the top UI bar.
class ZombieCardObject final : public StaticUIObject {
 public:
  // Places the selected zombie card image at its card slot.
  ZombieCardObject(ImageID imageID, int x, int y);

  // Asks the world to make this the only selected zombie card.
  void OnClick() override;

  // Applies or removes the selected visual state.
  void SetSelected(bool selected);

  // Returns whether this card is currently selected.
  bool IsSelected() const;

 private:
  // Whether this card has already entered the selected visual state.
  bool m_selected;
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
