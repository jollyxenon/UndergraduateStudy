#include "pvz/GameObject/UIObjects.hpp"

#include <algorithm>
#include <cmath>
#include <string>

#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Shared progress-meter anchor used by all progress-meter sprites.
constexpr int PROGRESS_METER_X = 717;
constexpr int PROGRESS_METER_Y = 578;

// Selected zombie cards rise slightly from their slot as visual feedback.
constexpr int ZOMBIE_CARD_SELECTED_Y_OFFSET = 5;

// Zombie cards stay unavailable for roughly two seconds after successful use.
constexpr int ZOMBIE_CARD_COOLDOWN_FRAMES = 2000 / MS_PER_FRAME;

// Converts a top-to-bottom lawn row index to its center Y coordinate in the
// engine's bottom-left coordinate system.
int GetBrainCenterY(int row) {
  return LAWN_GRID_TOP - row * LAWN_GRID_HEIGHT - LAWN_GRID_HEIGHT / 2;
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

// Cooldown masks cover cards and sit on the dedicated cooldown-mask layer.
CooldownMaskObject::CooldownMaskObject(int x, int y)
    : StaticUIObject(ImageID::COOLDOWN_MASK, x, y, LayerID::COOLDOWN_MASK,
                     SEED_WIDTH, SEED_HEIGHT),
      m_cardCenterY(y) {}

// The mask keeps its top edge fixed and shortens linearly as cooldown expires.
void CooldownMaskObject::SetRemainingRatio(double remainingRatio) {
  const double clampedRatio = std::clamp(remainingRatio, 0.0, 1.0);
  const int maskHeight =
      static_cast<int>(std::ceil(SEED_HEIGHT * clampedRatio));
  ResizeTo(SEED_WIDTH, maskHeight);
  MoveTo(GetX(), m_cardCenterY + (SEED_HEIGHT - maskHeight) / 2);
}

// Zombie cards use assets/zombie_card_*.png and live on the UI layer.
ZombieCardObject::ZombieCardObject(ImageID imageID, int x, int y)
    : StaticUIObject(imageID, x, y, LayerID::UI, SEED_WIDTH, SEED_HEIGHT),
      m_selected(false),
      m_cooldownFrames(0),
      m_cooldownMask(nullptr) {}

// Clicking a card asks the world to make it the only selected card.
void ZombieCardObject::OnClick() {
  if (IsCoolingDown()) {
    return;
  }

  std::shared_ptr<GameWorld> world = GetWorld();
  if (world) {
    world->SelectZombieCard(*this);
  }
}

// Cooling cards tick down once per frame and clear their overlay at the end.
void ZombieCardObject::Update() {
  if (!IsCoolingDown()) {
    return;
  }

  --m_cooldownFrames;
  if (m_cooldownMask) {
    m_cooldownMask->SetRemainingRatio(static_cast<double>(m_cooldownFrames) /
                                      ZOMBIE_CARD_COOLDOWN_FRAMES);
  }

  if (m_cooldownFrames <= 0 && m_cooldownMask) {
    m_cooldownMask->Kill();
    m_cooldownMask.reset();
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

// A successful deployment makes the card unavailable and shows a blocking mask.
void ZombieCardObject::StartCooldown() {
  SetSelected(false);
  m_cooldownFrames = ZOMBIE_CARD_COOLDOWN_FRAMES;

  if (m_cooldownMask && m_cooldownMask->IsAlive()) {
    return;
  }

  m_cooldownMask = std::make_shared<CooldownMaskObject>(GetX(), GetY());
  m_cooldownMask->SetRemainingRatio(1.0);
  std::shared_ptr<GameWorld> world = GetWorld();
  if (world) {
    world->AddObject(m_cooldownMask);
  }
}

// A positive frame counter means the card cannot currently be selected.
bool ZombieCardObject::IsCoolingDown() const { return m_cooldownFrames > 0; }

// Selection is cached so duplicate clicks do not move the card repeatedly.
bool ZombieCardObject::IsSelected() const { return m_selected; }

// The red line sprite marks where the current deployment area starts.
RedLineObject::RedLineObject(int x)
    : StaticUIObject(ImageID::RED_LINE, x, LAWN_GRID_CENTER_Y, LayerID::UI, 22,
                     502) {}

// Brain icons sit on the grass's left border, one centered in each row.
BrainObject::BrainObject(int row)
    : StaticUIObject(ImageID::BRAIN_ICON, LAWN_GRID_LEFT, GetBrainCenterY(row),
                     LayerID::UI, 32, 31) {}

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
