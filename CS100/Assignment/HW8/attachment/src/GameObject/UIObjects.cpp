#include "pvz/GameObject/UIObjects.hpp"

#include <string>

#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Shared progress-meter anchor used by all progress-meter sprites.
constexpr int PROGRESS_METER_X = 717;
constexpr int PROGRESS_METER_Y = 578;

// Selected zombie cards rise slightly from their slot as visual feedback.
constexpr int ZOMBIE_CARD_SELECTED_Y_OFFSET = 5;

// Converts a lawn row index to the center Y coordinate of a brain sitting on
// that row's bottom boundary line.
int GetBrainCenterY(int row) {
  const int rowBottom = LAWN_GRID_BOTTOM + row * LAWN_GRID_HEIGHT;
  return rowBottom + 31 / 2;
}

// Builds the visible sun text while keeping formatting in one place.
std::string MakeSunText(int sunAmount) { return std::to_string(sunAmount); }

}  // namespace

// Static UI images use no animation and keep a single hit point forever.
StaticUIObject::StaticUIObject(ImageID imageID, int x, int y, LayerID layer,
                               int width, int height)
    : GameObject(imageID, x, y, layer, width, height, AnimID::NO_ANIMATION) {}

// Static UI images are grouped under the UI category.
GameObjectType StaticUIObject::GetType() const { return GameObjectType::UI; }

// The background fills the whole window and must render behind all objects.
BackgroundObject::BackgroundObject()
    : StaticUIObject(ImageID::BACKGROUND, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                     LayerID::BACKGROUND, WINDOW_WIDTH, WINDOW_HEIGHT) {}

// Zombie cards use assets/zombie_card_*.png and live on the UI layer.
ZombieCardObject::ZombieCardObject(ImageID imageID, int x, int y)
    : StaticUIObject(imageID, x, y, LayerID::UI, SEED_WIDTH, SEED_HEIGHT),
      m_selected(false) {}

// Clicking a card asks the world to make it the only selected card.
void ZombieCardObject::OnClick() {
  std::shared_ptr<GameWorld> world = GetWorld();
  if (world) {
    world->SelectZombieCard(*this);
  }
}

// Changing selection moves the card exactly once in either direction.
void ZombieCardObject::SetSelected(bool selected) {
  if (m_selected == selected) {
    return;
  }
  m_selected = selected;
  const int yOffset =
      selected ? ZOMBIE_CARD_SELECTED_Y_OFFSET : -ZOMBIE_CARD_SELECTED_Y_OFFSET;
  MoveTo(GetX(), GetY() + yOffset);
}

// Selection is cached so duplicate clicks do not move the card repeatedly.
bool ZombieCardObject::IsSelected() const { return m_selected; }

// The red line sprite marks where the current deployment area starts.
RedLineObject::RedLineObject(int x)
    : StaticUIObject(ImageID::RED_LINE, x, LAWN_GRID_CENTER_Y, LayerID::UI, 22,
                     502) {}

// Brain icons sit on each row's bottom line in the leftmost lawn column.
BrainObject::BrainObject(int row)
    : StaticUIObject(ImageID::BRAIN_ICON, FIRST_COL_CENTER,
                     GetBrainCenterY(row), LayerID::UI, 32, 31) {}

// Progress-meter sprites use assets/flag_meter_*.png in the top-right corner.
ProgressMeterObject::ProgressMeterObject(ImageID imageID)
    : StaticUIObject(imageID, PROGRESS_METER_X, PROGRESS_METER_Y, LayerID::UI,
                     158, 24) {}

// The text counter uses a dark color matching the original UI palette.
SunCounterText::SunCounterText(int sunAmount)
    : TextBase(SUN_COUNTER_X, SUN_COUNTER_Y, MakeSunText(sunAmount), 0.0, 0.0,
               0.0, true) {}

// Updating the sun counter only changes its visible text.
void SunCounterText::SetSunAmount(int sunAmount) {
  SetText(MakeSunText(sunAmount));
}
