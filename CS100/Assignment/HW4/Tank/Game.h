#pragma once

/// \file
/// \brief This file contains the game lifecycle and logics.
/// There are 5 important functions:
/// `GameInit`, `GameInput`, `GameUpdate`, `GameTerminate`, and
/// the most important: `GameLifecycle`.
/// Please read the corresponding comments to understand their relationships.

//
//
//
//
//
#include "Config.h"
#include "Renderer.h"

#include <stdlib.h>
#include <time.h>

typedef struct {
  char keyHit; // The keyboard key hit by the player at this frame.
} Game;

// The game singleton.
static Game PlayerGame;

static Game EnemyGame;

static int skillSpawnCooldown;

// The keyboard key "ESC".
static const char keyESC = '\033';

void EnemyGameInput(void);

int RandSkillSpawnCooldown(void) {
  return config.fps * (Rand(6) + 5);
}

int GetTankShootCooldown(const Tank *tank) {
  int cooldown = tank->isPlayer ? config.PlayerShootCooldown : config.EnemyShootCooldown;
  if (tank->weaponMode == eWeaponRapidFire)
    cooldown = cooldown > 1 ? cooldown / 2 : 1;
  else if (tank->weaponMode == eWeaponBreaker)
    cooldown *= 3;
  return cooldown;
}

void ApplySkillToTank(Tank *tank, SkillType type) {
  if (type == eSkillShield)
    tank->shieldCharges = 1;
  else if (type == eSkillRapidFire)
    tank->weaponMode = eWeaponRapidFire;
  else if (type == eSkillBreaker)
    tank->weaponMode = eWeaponBreaker;
}

void SpawnSkill(void) {
  int maxAttempts = map.size.x * map.size.y * 4;
  for (int attempt = 0; attempt < maxAttempts; ++attempt) {
    Vec pos = RandPos();
    if (IsSkillSpawnBlocked(pos))
      continue;

    Skill *skill = RegNew(regSkill);
    skill->pos = pos;

    int randomSkill = Rand(3);
    if (randomSkill == 0)
      skill->type = eSkillShield;
    else if (randomSkill == 1)
      skill->type = eSkillRapidFire;
    else
      skill->type = eSkillBreaker;
    return;
  }
}

void TryPickupSkills(Tank *tank) {
  for (RegIterator it = RegBegin(regSkill); it != RegEnd(regSkill); it = RegNext(it)) {
    Skill *skill = RegEntry(regSkill, it);
    if (TankOccupiesPos(tank, skill->pos)) {
      Vec pos = skill->pos;
      ApplySkillToTank(tank, skill->type);
      RegDelete(skill);
      RdrPutChar(pos, map.flags[Idx(pos)], TK_AUTO_COLOR);
    }
  }
}

/// \brief Check whether `key` is one of the movement keys.
bool IsMoveKey(char key) {
  return key == 'w' || key == 'a' || key == 's' || key == 'd';
}

/// \brief Convert a movement key to its corresponding direction.
Dir KeyToDir(char key) {
  if (key == 'w')
    return eDirUP;
  if (key == 's')
    return eDirDN;
  if (key == 'a')
    return eDirLF;
  if (key == 'd')
    return eDirRT;
  return eDirInvalid;
}

/// \brief Create a bullet directly in front of `tank`.
void SpawnBulletFromTank(const Tank *tank) {
  Bullet *bullet = RegNew(regBullet);
  bullet->pos = Add(tank->pos, DirToVec(tank->dir));
  bullet->dir = tank->dir;
  bullet->color = tank->color;
  bullet->isPlayer = tank->isPlayer;
  bullet->breaksWalls = tank->weaponMode == eWeaponBreaker;
  bullet->moveCooldown = 0;
}

/// \brief Move `tank` according to the current input if movement is allowed.
void TryMoveTank(Tank *tank, Game *game) {
  if (!IsMoveKey(game->keyHit))
    return;

  tank->dir = KeyToDir(game->keyHit);
  Vec targetPos = Add(tank->pos, DirToVec(tank->dir));
  if (is3x3Overlap(targetPos, eOverlapSolid | eOverlapWall | eOverlapTank, tank))
    return;

  tank->moveCooldown = tank->isPlayer ? config.PlayerMoveCooldown : config.EnemyMoveCooldown;
  tank->pos = targetPos;
  game->keyHit = '\0';
}

/// \brief Let `tank` shoot when the shoot key is pressed and cooldown ends.
void TryShootTank(Tank *tank, Game *game) {
  tank->shootCooldown = tank->shootCooldown > 0 ? tank->shootCooldown - 1 : 0;
  if (game->keyHit != 'k' || tank->shootCooldown != 0)
    return;

  SpawnBulletFromTank(tank);
  tank->shootCooldown = GetTankShootCooldown(tank);
  game->keyHit = '\0';
}

/// \brief Update movement, pickups, and shooting for a single tank.
void UpdateTank(Tank *tank) {
  Game *game = tank->isPlayer ? &PlayerGame : &EnemyGame;

  tank->moveCooldown = tank->moveCooldown > 0 ? tank->moveCooldown - 1 : 0;
  if (tank->moveCooldown == 0) {
    if (!tank->isPlayer)
      EnemyGameInput();
    TryMoveTank(tank, game);
    TryPickupSkills(tank);
  }

  TryShootTank(tank, game);
}

/// \brief Advance one bullet and handle collisions with map blocks.
void UpdateBullet(Bullet *bullet) {
  Vec targetPos = Add(bullet->pos, DirToVec(bullet->dir));

  bullet->moveCooldown = bullet->moveCooldown > 0 ? bullet->moveCooldown - 1 : 0;
  if (bullet->moveCooldown == 0) {
    bullet->moveCooldown = config.BulletMoveCooldown;
    bullet->pos = targetPos;
  }

  if (map.flags[Idx(bullet->pos)] == eFlagSolid) {
    RegDelete(bullet);
  } else if (map.flags[Idx(bullet->pos)] == eFlagWall) {
    map.flags[Idx(bullet->pos)] = eFlagNone;
    RdrPutChar(bullet->pos, eFlagNone, TK_AUTO_COLOR);
    if (!bullet->breaksWalls)
      RegDelete(bullet);
  }
}

/// \brief Handle all bullet-versus-tank collisions for the current frame.
void ResolveBulletTankCollisions(void) {
  for (RegIterator itTank = RegBegin(regTank); itTank != RegEnd(regTank); itTank = RegNext(itTank)) {
    Tank *tank = RegEntry(regTank, itTank);
    bool tankDestroyed = false;

    for (RegIterator itBullet = RegBegin(regBullet); itBullet != RegEnd(regBullet); itBullet = RegNext(itBullet)) {
      Bullet *bullet = RegEntry(regBullet, itBullet);
      if (!TankOccupiesPos(tank, bullet->pos) || bullet->isPlayer == tank->isPlayer)
        continue;

      RegDelete(bullet);
      if (tank->shieldCharges > 0) {
        tank->shieldCharges--;
      } else {
        RegDelete(tank);
        tankDestroyed = true;
      }
      break;
    }

    if (tankDestroyed)
      continue;
  }
}

//
//
//
/// \brief Configure the scene (See `Scene.h`) with `config` (See `Config.h`), and
/// perform initializations including:
/// 1. Terminal setup.
/// 2. Memory allocations.
/// 3. Map and object generations.
/// 4. Rendering of the initialized scene.
///
/// \note This function should be called at the very beginning of `GameLifecycle`.
void GameInit(void) {
  // Setup terminal.
  TermSetupGameEnvironment();
  TermClearScreen();

  // Configure scene.
  map.size = config.mapSize;
  int nEnemies = config.nEnemies;
  int nSolids = config.nSolids;
  int nWalls = config.nWalls;

  // Initialize scene.
  RegInit(regTank);
  RegInit(regBullet);
  RegInit(regSkill);

  PlayerGame.keyHit = '\0';
  EnemyGame.keyHit = '\0';
  skillSpawnCooldown = RandSkillSpawnCooldown();

  map.flags = (Flag *)malloc(sizeof(Flag) * map.size.x * map.size.y);
  // Set the borders of the map to solid and the rest to empty.
  for (int y = 0; y < map.size.y; ++y)
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};

      Flag flag = eFlagNone;
      if (x == 0 || y == 0 || x == map.size.x - 1 || y == map.size.y - 1)
        flag = eFlagSolid;

      map.flags[Idx(pos)] = flag;
    }

  // Player's tank generation
  {
    Tank *tank = RegNew(regTank);
    do {
      tank->pos = RandVec(map.size);
    } while (is3x3Overlap(tank->pos, eOverlapSolid | eOverlapWall, tank));
    tank->dir = (Dir)Rand(4);
    tank->color = TK_GREEN;
    tank->isPlayer = true;
    tank->shieldCharges = 0;
    tank->weaponMode = eWeaponNormal;
    tank->moveCooldown = 0;
    tank->shootCooldown = config.PlayerShootCooldown;
  }

  // Enemy's tanks generation
  {
    for (int i = 0; i < nEnemies; ++i) {
      Tank *tank = RegNew(regTank);
      do {
        tank->pos = RandVec(map.size);
      } while (is3x3Overlap(tank->pos, eOverlapSolid | eOverlapWall | eOverlapTank, tank));

      tank->dir = (Dir)Rand(4);
      tank->color = TK_RED;
      tank->isPlayer = false;
      tank->shieldCharges = 0;
      tank->weaponMode = eWeaponNormal;
      tank->moveCooldown = config.EnemyMoveCooldown;
      tank->shootCooldown = config.EnemyShootCooldown;
    }
  }

  // Solid generation
  {
    for (int num = 0; num < nSolids;) {
      Vec pos = RandVec(map.size);
      if (!is3x3Overlap(pos, eOverlapSolid | eOverlapWall | eOverlapTank, NULL)) {
        for (int i = -1; i <= 1; i++) {
          for (int j = -1; j <= 1; j++) {
            Vec temp_pos = {pos.x + i, pos.y + j};
            map.flags[Idx(temp_pos)] = eFlagSolid;
          }
        }
        ++num;
      }
    }
  }

  // Wall generation
  {
    for (int num = 0; num < nWalls;) {
      Vec pos = RandVec(map.size);
      if (!is3x3Overlap(pos, eOverlapSolid | eOverlapWall | eOverlapTank, NULL)) {
        for (int i = -1; i <= 1; i++) {
          for (int j = -1; j <= 1; j++) {
            Vec temp_pos = {pos.x + i, pos.y + j};
            map.flags[Idx(temp_pos)] = eFlagWall;
          }
        }
        ++num;
      }
    }
  }

  // Initialize renderer.
  renderer.csPrev = (char *)malloc(sizeof(char) * map.size.x * map.size.y);
  renderer.colorsPrev = (Color *)malloc(sizeof(Color) * map.size.x * map.size.y);
  renderer.cs = (char *)malloc(sizeof(char) * map.size.x * map.size.y);
  renderer.colors = (Color *)malloc(sizeof(Color) * map.size.x * map.size.y);

  for (int i = 0; i < map.size.x * map.size.y; ++i) {
    renderer.csPrev[i] = renderer.cs[i] = ' ';
    renderer.colorsPrev[i] = renderer.colors[i] = TK_NORMAL;
  }

  // Render scene.
  for (int y = 0; y < map.size.y; ++y)
    for (int x = 0; x < map.size.x; ++x) {
      Vec pos = {x, y};
      RdrPutChar(pos, map.flags[Idx(pos)], TK_AUTO_COLOR);
    }
  RdrRender();
  RdrFlush();
}

//
//
//
/// \brief Read input from the player.
///
/// \note This function should be called in the loop of `GameLifecycle` before `GameUpdate`.
void GameInput(void) {
  char temp_keyHit = kbhit_t() ? (char)getch_t() : '\0';
  if (temp_keyHit == keyESC || temp_keyHit == 'w' || temp_keyHit == 'a' || temp_keyHit == 's' || temp_keyHit == 'd' ||
      temp_keyHit == 'k') {
    PlayerGame.keyHit = temp_keyHit;
  }
}

/// \brief Generate enemy's input.
void EnemyGameInput(void) {
  char temp_keyHit;
  int randomNum = Rand(6);
  if (randomNum == 0)
    temp_keyHit = 'w';
  else if (randomNum == 1)
    temp_keyHit = 'a';
  else if (randomNum == 2)
    temp_keyHit = 's';
  else if (randomNum == 3)
    temp_keyHit = 'd';
  else if (randomNum == 4)
    temp_keyHit = 'k';
  else
    temp_keyHit = '\0';
  EnemyGame.keyHit = temp_keyHit;
  if (temp_keyHit == keyESC || temp_keyHit == 'w' || temp_keyHit == 'a' || temp_keyHit == 's' || temp_keyHit == 'd' ||
      temp_keyHit == 'k') {
    EnemyGame.keyHit = temp_keyHit;
  }
}

//
//
//
/// \brief Perform all tasks required for a frame update, including:
/// 1. Game logics of `Tank`s, `Bullet`s, etc.
/// 2. Rerendering all objects in the scene.
///
/// \note This function should be called in the loop of `GameLifecycle` after `GameInput`.
void GameUpdate(void) {
  RdrClear();

  skillSpawnCooldown = skillSpawnCooldown > 0 ? skillSpawnCooldown - 1 : 0;
  if (skillSpawnCooldown == 0) {
    SpawnSkill();
    skillSpawnCooldown = RandSkillSpawnCooldown();
  }

  // Update tanks.
  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
    Tank *tank = RegEntry(regTank, it);
    UpdateTank(tank);
  }

  // Update bullets.
  for (RegIterator it = RegBegin(regBullet); it != RegEnd(regBullet); it = RegNext(it)) {
    Bullet *bullet = RegEntry(regBullet, it);
    UpdateBullet(bullet);
  }

  ResolveBulletTankCollisions();

  RdrRender();
  RdrFlush();
}

//
//
//
/// \brief Terminate the game and free all the resources.
///
/// \note This function should be called at the very end of `GameLifecycle`.
void GameTerminate(void) {
  while (RegSize(regTank) > 0)
    RegDelete(RegEntry(regTank, RegBegin(regTank)));

  while (RegSize(regBullet) > 0)
    RegDelete(RegEntry(regBullet, RegBegin(regBullet)));

  while (RegSize(regSkill) > 0)
    RegDelete(RegEntry(regSkill, RegBegin(regSkill)));

  free(map.flags);

  free(renderer.csPrev);
  free(renderer.colorsPrev);
  free(renderer.cs);
  free(renderer.colors);

  TermClearScreen();
}

//
//
//
/// \brief Lifecycle of the game, defined by calling the 4 important functions:
/// `GameInit`, `GameInput`, `GameUpdate`, and `GameTerminate`.
///
/// \note This function should be called by `main`.
void GameLifecycle(void) {
  GameInit();

  double frameTime = (double)1000 / (double)config.fps;
  clock_t frameBegin = clock();

  while (true) {
    GameInput();
    if (PlayerGame.keyHit == keyESC)
      break;

    GameUpdate();

    while (((double)(clock() - frameBegin) / CLOCKS_PER_SEC) * 1000.0 < frameTime - 0.5)
      Daze();
    frameBegin = clock();

    int enemyCount = 0, playerCount = 0;
    for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
      Tank *tank = RegEntry(regTank, it);
      if (tank->isPlayer) {
        playerCount++;
      } else {
        enemyCount++;
      }
    }
    if (playerCount == 0 || enemyCount == 0) {
      Sleep(1000);
      break;
    }
  }

  GameTerminate();
}
