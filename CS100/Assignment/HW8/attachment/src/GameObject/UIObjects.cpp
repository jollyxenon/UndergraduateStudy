#include "pvz/GameObject/UIObjects.hpp"

#include <cmath>
#include <string>

#include "pvz/GameObject/ZombieObjects.hpp"
#include "pvz/GameWorld/GameWorld.hpp"

namespace {

// Shared progress-meter anchor used by all progress-meter sprites.
constexpr int PROGRESS_METER_X = 717;
constexpr int PROGRESS_METER_Y = 578;

// Selected zombie cards rise slightly from their slot as visual feedback.
constexpr int ZOMBIE_CARD_SELECTED_Y_OFFSET = 5;

// Zombie cards stay unavailable for roughly two seconds after successful use.
constexpr int ZOMBIE_CARD_COOLDOWN_FRAMES = 2000 / MS_PER_FRAME;

// Regular zombies cost two small sun units in I, Zombie mode.
constexpr int REGULAR_ZOMBIE_SUN_COST = 50;

// Bucket-head zombies cost more because the bucket gives them armor HP.
constexpr int BUCKET_HEAD_ZOMBIE_SUN_COST = 125;

// Bungee zombies cost the same as bucket-head zombies in this implementation.
constexpr int BUNGEE_ZOMBIE_SUN_COST = 125;

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
  const int maskHeight =
      static_cast<int>(std::ceil(SEED_HEIGHT * remainingRatio));
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

  GetWorld().SelectZombieCard(*this);
}

// Cooling cards tick down once per frame and clear their overlay at the end.
void ZombieCardObject::Update() {
  if (!IsCoolingDown()) {
    return;
  }

  --m_cooldownFrames;
  m_cooldownMask->SetRemainingRatio(static_cast<double>(m_cooldownFrames) /
                                    ZOMBIE_CARD_COOLDOWN_FRAMES);

  if (m_cooldownFrames <= 0) {
    m_cooldownMask->Kill();
    m_cooldownMask = nullptr;
  }
}

// Changing selection moves the card exactly once in either direction.
void ZombieCardObject::SetSelected(bool selected) {
  m_selected = selected;
  const int yOffset =
      selected ? ZOMBIE_CARD_SELECTED_Y_OFFSET : -ZOMBIE_CARD_SELECTED_Y_OFFSET;
  MoveTo(GetX(), GetY() + yOffset);
}

// A successful deployment makes the card unavailable and shows a blocking mask.
void ZombieCardObject::StartCooldown() {
  SetSelected(false);
  m_cooldownFrames = ZOMBIE_CARD_COOLDOWN_FRAMES;
  std::shared_ptr<CooldownMaskObject> cooldownMask =
      std::make_shared<CooldownMaskObject>(GetX(), GetY());
  m_cooldownMask = cooldownMask.get();
  m_cooldownMask->SetRemainingRatio(1.0);
  GetWorld().AddObject(cooldownMask);
}

// Stage resets make the card immediately usable and remove any old overlay.
void ZombieCardObject::ResetCooldown() {
  SetSelected(false);
  m_cooldownFrames = 0;
  if (m_cooldownMask) {
    m_cooldownMask->Kill();
    m_cooldownMask = nullptr;
  }
}

// A positive frame counter means the card cannot currently be selected.
bool ZombieCardObject::IsCoolingDown() const { return m_cooldownFrames > 0; }

// Selection is cached so duplicate clicks do not move the card repeatedly.
bool ZombieCardObject::IsSelected() const { return m_selected; }

// Most zombie cards cannot be placed into the protected plant area.
bool ZombieCardObject::CanPlaceBeforeRedLine() const { return false; }

// Regular zombie cards use the regular zombie card art.
RegularZombieCardObject::RegularZombieCardObject(int x, int y)
    : ZombieCardObject(ImageID::ZOMBIE_CARD_REGULAR, x, y) {}

// Regular zombies use the base deployment cost.
int RegularZombieCardObject::GetSunCost() const {
  return REGULAR_ZOMBIE_SUN_COST;
}

// Placement creates one regular zombie in the chosen lawn cell.
std::shared_ptr<GameObject> RegularZombieCardObject::CreateZombie(
    int row, int col) const {
  return std::make_shared<RegularZombieObject>(row, col);
}

// Bucket-head zombie cards use the bucket card art.
BucketHeadZombieCardObject::BucketHeadZombieCardObject(int x, int y)
    : ZombieCardObject(ImageID::ZOMBIE_CARD_BUCKET, x, y) {}

// Bucket-head zombies cost more than regular zombies.
int BucketHeadZombieCardObject::GetSunCost() const {
  return BUCKET_HEAD_ZOMBIE_SUN_COST;
}

// Placement creates one bucket-head zombie in the chosen lawn cell.
std::shared_ptr<GameObject> BucketHeadZombieCardObject::CreateZombie(
    int row, int col) const {
  return std::make_shared<BucketHeadZombieObject>(row, col);
}

// Bungee zombie cards use the bungee card art.
BungeeZombieCardObject::BungeeZombieCardObject(int x, int y)
    : ZombieCardObject(ImageID::ZOMBIE_CARD_BUNGEE, x, y) {}

// Bungee zombies may target any grass cell, including cells before the red
// line.
bool BungeeZombieCardObject::CanPlaceBeforeRedLine() const { return true; }

// Bungee zombies use the requested deployment cost.
int BungeeZombieCardObject::GetSunCost() const {
  return BUNGEE_ZOMBIE_SUN_COST;
}

// Placement creates one bungee zombie in the chosen lawn cell.
std::shared_ptr<GameObject> BungeeZombieCardObject::CreateZombie(
    int row, int col) const {
  return std::make_shared<BungeeZombieObject>(row, col);
}

// The red line sprite marks where the current deployment area starts.
RedLineObject::RedLineObject(int x)
    : StaticUIObject(ImageID::RED_LINE, x, LAWN_GRID_CENTER_Y, LayerID::UI, 22,
                     502) {}

// Brain icons sit on the grass's left border, one centered in each row.
BrainObject::BrainObject(int row)
    : StaticUIObject(ImageID::BRAIN_ICON, LAWN_GRID_LEFT, GetBrainCenterY(row),
                     LayerID::UI, 32, 31) {
  SetGridPosition(row, 0);
}

// Brain targets are queried separately from other static UI sprites.
GameObjectType BrainObject::GetType() const { return GameObjectType::BRAIN; }

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
