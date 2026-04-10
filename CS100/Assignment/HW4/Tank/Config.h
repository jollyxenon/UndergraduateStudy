#pragma once

/// \file
/// \brief This file contains the game configurations.
/// The singleton `config` should be set before calling `GameLifecycle`, and
/// will be used to configure the scene in `GameInit`.

//
//
//
//
//
#include "Base.h"

/// \brief The game configurations.
///
/// \note The singleton `config` should be set before calling `GameLifecycle`, and
/// will be used to configure the scene in `GameInit`.
typedef struct {
  int fps;                // Frame rate or frames per second (FPS) of the game.
  Vec mapSize;            // Width (x) and height (y) of the map.
  int nEnemies;           // Number of enemy tanks.
  int nSolids;            // Number of solids.
  int nWalls;             // Number of walls.
  int PlayerMoveCooldown; // Frames to wait before player can move again.
  int EnemyMoveCooldown;  // Frames to wait before enemy can move again.
} Config;

// The config singleton.
static Config config;
