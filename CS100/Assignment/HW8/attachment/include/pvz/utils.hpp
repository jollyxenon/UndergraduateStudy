#ifndef UTILS_HPP__
#define UTILS_HPP__

#include <cassert>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

inline std::string FindAssetDir() {
  const std::filesystem::path cwd = std::filesystem::current_path();
  std::vector<std::filesystem::path> candidates = {
      cwd / "assets",
      cwd / "../assets",
      cwd / "../../assets",
      cwd / "attachment/assets",
  };

#ifdef PVZ_SOURCE_DIR
  candidates.push_back(std::filesystem::path(PVZ_SOURCE_DIR) / "assets");
#endif

  for (const auto& candidate : candidates) {
    std::error_code existsError;
    if (!std::filesystem::exists(candidate / "background.png", existsError)) {
      continue;
    }

    std::error_code canonicalError;
    const auto canonical =
        std::filesystem::weakly_canonical(candidate, canonicalError);
    return (canonicalError ? candidate : canonical).string();
  }

  return "assets";
}

static const std::string ASSET_DIR = FindAssetDir();

// Returns a random integer within [min, max] (inclusive).
inline int randInt(int min, int max) {
  if (max < min)
    std::swap(max, min);
  static std::random_device rd;
  static std::mt19937 generator(rd());
  std::uniform_int_distribution<> distro(min, max);
  return distro(generator);
}

enum class LevelStatus { ONGOING, WINNING, LOSING };

enum class KeyCode {
  NONE,
  ENTER, // Enter
  QUIT   // Esc
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int LAWN_GRID_WIDTH = 80;
const int LAWN_GRID_HEIGHT = 100;

const int FIRST_ROW_CENTER = 75;
const int FIRST_COL_CENTER = 75;
const int GAME_ROWS = 5;
const int GAME_COLS = 9;
const int LAWN_GRID_LEFT = FIRST_COL_CENTER - LAWN_GRID_WIDTH / 2;
const int LAWN_GRID_RIGHT = FIRST_COL_CENTER + (GAME_COLS - 1) * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2;
const int LAWN_GRID_BOTTOM = FIRST_ROW_CENTER - LAWN_GRID_HEIGHT / 2;
const int LAWN_GRID_TOP = FIRST_ROW_CENTER + (GAME_ROWS - 1) * LAWN_GRID_HEIGHT + LAWN_GRID_HEIGHT / 2;
const int LAWN_GRID_CENTER_Y = (LAWN_GRID_BOTTOM + LAWN_GRID_TOP) / 2;
const int TOTAL_ROUNDS = 5;
const int INITIAL_ZOMBIE_DEPLOYMENT_START_COL = 1;
const int FINAL_ZOMBIE_DEPLOYMENT_START_COL =
    INITIAL_ZOMBIE_DEPLOYMENT_START_COL + TOTAL_ROUNDS - 1;
const int ZOMBIE_DEPLOYMENT_BUFFER_COLS = 1;
const int ZOMBIE_DEPLOYMENT_WIDTH = LAWN_GRID_WIDTH;

const int SEED_WIDTH = 50;
const int SEED_HEIGHT = 70;
const int ZOMBIE_CARD_FIRST_X = 130;
const int ZOMBIE_CARD_SPACING = 60;
const int ZOMBIE_CARD_Y = WINDOW_HEIGHT - 44;
const int SUN_COUNTER_X = 58;
const int SUN_COUNTER_Y = WINDOW_HEIGHT - 82;
const int SUN_VALUE = 25;

enum class ImageID {
  NONE = 0,
  BACKGROUND,
  SUN,
  SHOVEL,
  COOLDOWN_MASK,
  SUNFLOWER = 10,
  PEASHOOTER,
  WALLNUT,
  REPEATER,
  WALLNUT_CRACKED,
  RED_REPEATER,
  SEED_SUNFLOWER = 20,
  SEED_PEASHOOTER,
  SEED_WALLNUT,
  SEED_REPEATER,
  SEED_RED_REPEATER,
  REGULAR_ZOMBIE = 30,
  CONEHEAD_ZOMBIE,
  BUCKET_HEAD_ZOMBIE,
  POLE_VAULTING_ZOMBIE,
  BUNGEE_ZOMBIE,
  BUNGEE_ZOMBIE_GRAB,
  PEA = 40,
  RED_PEA,
  ZOMBIES_WON = 100,
  BRAIN_ICON = 110,
  RED_LINE,
  PROGRESS_METER_EMPTY,
  PROGRESS_METER_FULL,
  PROGRESS_METER_FILL,
  PROGRESS_METER_STAGE_1,
  PROGRESS_METER_STAGE_2,
  PROGRESS_METER_STAGE_3,
  PROGRESS_METER_STAGE_4,
  PROGRESS_METER_STAGE_5,
  PROGRESS_METER_STAGE_6,
  PROGRESS_METER_STAGE_7,
  PROGRESS_METER_STAGE_8,
  ZOMBIE_CARD_REGULAR,
  ZOMBIE_CARD_CONEHEAD,
  ZOMBIE_CARD_POLE,
  ZOMBIE_CARD_BUCKET,
  ZOMBIE_CARD_BUNGEE
};

enum class AnimID { NO_ANIMATION = 0, IDLE, WALK, EAT, RUN, JUMP };

const int MAX_LAYERS = 7;

enum class LayerID {
  SUN = 0,
  ZOMBIES,
  PROJECTILES,
  PLANTS,
  COOLDOWN_MASK,
  UI,
  BACKGROUND,
};

const int MS_PER_FRAME = 33;

#endif // !UTILS_HPP__
