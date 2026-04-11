#pragma once

/// \file
/// \brief This file contains the definitions, singletons, and functions of
/// the game-related types, such as `Tank`, `Bullet`, and `Map`.
///
/// The powerful "registries" are intensively used to make our life easier.
/// Please read the comments of `Registry.h` before continue.

//
//
//
//
//
#include "Registry.h"
#include "Terminal.h"

/// \brief Enums of the 2D directions.
typedef enum {
  eDirDN, // Down.
  eDirLF, // Left.
  eDirRT, // Right.
  eDirUP, // Up.

  eDirInvalid, // Invalid.
} Dir;

/// \brief Enums of the map flags.
typedef enum {
  eFlagNone = ' ',  // Nothing here, passable.
  eFlagSolid = '%', // Solid, impassable and non-destructible.
  eFlagWall = '#',  // Wall, impassable but destructible.

  eFlagTank = 'T', // Tank, impassable but destructible.

  eFlagInvalid = '\0', // Invalid.
} Flag;

/// \brief Enums of collectible skill types.
typedef enum {
  eSkillShield = '=',
  eSkillRapidFire = 'O',
  eSkillBreaker = '!',

  eSkillInvalid = '\0',
} SkillType;

/// \brief Enums of weapon modes.
typedef enum {
  eWeaponNormal,
  eWeaponRapidFire,
  eWeaponBreaker,
} WeaponMode;

/// \example It is easy to create or delete a `Tank` with the help of registries, see `Registry.h`.
/// ```c
/// Tank *tank = RegNew(regTank); //! `malloc` is called here.
/// tank->pos = ...
/// ...
/// RegDelete(tank); //! `free` is called here.
/// ```
typedef struct {
  TK_REG_AUTH;       // Authorize `Tank` to make it compatible with registries, see `Registry.h`.
  Vec pos;           // Position.
  Dir dir;           // Direction.
  Color color;       // Color of the tank and its bullets.
  bool isPlayer;     // Whether this tank is player or enemy.
  int shieldCharges; // Remaining shield blocks.
  WeaponMode weaponMode;
  int moveCooldown;  // Frames to wait before next move.
  int shootCooldown; // Frames to wait before next shoot.
} Tank;

/// \example It is easy to create or delete a `Bullet` with the help of registries, see `Registry.h`.
/// ```c
/// Bullet *bullet = RegNew(regBullet); //! `malloc` is called here.
/// bullet->pos = ...
/// ...
/// RegDelete(bullet); //! `free` is called here.
/// ```
typedef struct {
  TK_REG_AUTH;      // Authorize `Bullet` to make it compatible with registries, see `Registry.h`.
  Vec pos;          // Position.
  Dir dir;          // Direction.
  Color color;      // Color.
  bool isPlayer;    // Whether this bullet was shot by player or enemy.
  bool breaksWalls; // Whether this bullet can destroy walls without disappearing.
  int moveCooldown; // Frames to wait before next move.
} Bullet;

typedef struct {
  TK_REG_AUTH;
  Vec pos;
  SkillType type;
} Skill;

typedef struct {
  // Width (x) and height (y) of the map.
  Vec size;
  // The flags of every positions of the map.
  //! Note that only static flags such as `eFlagNone` and `eFlagSolid` are included here.
  //! Dynamic flags such as `eFlagTank` are not included and will be set to `eFlagNone`.
  Flag *flags;
} Map;

//
//
//
// Define a registry for `Tank`, see `Registry.h`.
static TK_REG_DEF(Tank, regTank);

// Define a registry for `Bullet`, see `Registry.h`.
static TK_REG_DEF(Bullet, regBullet);

// Define a registry for `Skill`, see `Registry.h`.
static TK_REG_DEF(Skill, regSkill);

// The map singleton.
static Map map;

//
//
//
/// \brief Convert `pos` to its linearized index of the map.
///
/// \example ```c
/// Vec pos = {x, y};
/// Flag flag = map.flags[Idx(pos)];
/// ```
int Idx(Vec pos) {
  return pos.x + pos.y * map.size.x;
}

/// \brief Check whether `pos` is inside the map.
bool IsInsideMap(Vec pos) {
  return 0 <= pos.x && pos.x < map.size.x && 0 <= pos.y && pos.y < map.size.y;
}

/// \brief Convert a direction to a vector.
Vec DirToVec(Dir dir) {
  if (dir == eDirDN)
    return (Vec){0, -1};
  else if (dir == eDirUP)
    return (Vec){0, 1};
  else if (dir == eDirLF)
    return (Vec){-1, 0};
  else if (dir == eDirRT)
    return (Vec){1, 0};
  else
    return (Vec){0, 0};
}

/// \brief Enums of overlap types for `is3x3Overlap`.
typedef enum { eOverlapNone = 0, eOverlapSolid = 1 << 0, eOverlapWall = 1 << 1, eOverlapTank = 1 << 2 } OverlapType;

/// \brief Check whether `pos` lies inside the 3x3 area centered at `center`.
bool IsWithin3x3(Vec center, Vec pos) {
  return pos.x >= center.x - 1 && pos.x <= center.x + 1 && pos.y >= center.y - 1 && pos.y <= center.y + 1;
}

/// \brief Check whether the 3x3 areas centered at `lhs` and `rhs` overlap.
bool TanksOverlap3x3(Vec lhs, Vec rhs) {
  return rhs.x >= lhs.x - 2 && rhs.x <= lhs.x + 2 && rhs.y >= lhs.y - 2 && rhs.y <= lhs.y + 2;
}

/// \brief Detect whether a 3x3 area around `pos` overlaps with the given types.
bool is3x3Overlap(Vec pos, int overlapTypes, const void *ignoreObj) {
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      Vec temp_pos = Add(pos, (Vec){i, j});
      if (!IsInsideMap(temp_pos))
        return true;
      Flag flag = map.flags[Idx(temp_pos)];
      if ((overlapTypes & eOverlapSolid) && flag == eFlagSolid)
        return true;
      if ((overlapTypes & eOverlapWall) && flag == eFlagWall)
        return true;
    }
  }

  if (overlapTypes & eOverlapTank) {
    for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
      Tank *tank = RegEntry(regTank, it);
      if (tank == ignoreObj)
        continue;
      if (TanksOverlap3x3(tank->pos, pos))
        return true;
    }
  }

  return false;
}

/// \brief Move cursor to `pos`.
///
/// \example ```c
/// MoveCursor(pos);
/// printf(TK_TEXT("Hello World!\n", TK_RED));
/// ```
void MoveCursor(Vec pos) {
  int row = map.size.y - 1 - pos.y;
  int col = pos.x * 2;
  TermMoveCursor(row, col);
}

/// \brief Randomly generate a position in map.
Vec RandPos(void) {
  return RandVec(map.size);
}

/// \brief Check whether `pos` is covered by `tank`'s 3x3 body.
bool TankOccupiesPos(const Tank *tank, Vec pos) {
  return IsWithin3x3(tank->pos, pos);
}

Skill *SkillAt(Vec pos) {
  for (RegIterator it = RegBegin(regSkill); it != RegEnd(regSkill); it = RegNext(it)) {
    Skill *skill = RegEntry(regSkill, it);
    if (Eq(skill->pos, pos))
      return skill;
  }
  return NULL;
}

bool IsSkillSpawnBlocked(Vec pos) {
  if (map.flags[Idx(pos)] != eFlagNone)
    return true;

  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
    Tank *tank = RegEntry(regTank, it);
    if (TankOccupiesPos(tank, pos))
      return true;
  }

  for (RegIterator it = RegBegin(regBullet); it != RegEnd(regBullet); it = RegNext(it)) {
    Bullet *bullet = RegEntry(regBullet, it);
    if (Eq(bullet->pos, pos))
      return true;
  }

  return SkillAt(pos) != NULL;
}

char SceneBaseCharAt(Vec pos) {
  Skill *skill = SkillAt(pos);
  if (skill != NULL)
    return (char)skill->type;
  return (char)map.flags[Idx(pos)];
}
