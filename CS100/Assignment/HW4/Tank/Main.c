/// \file
/// \brief Welcome to CS100 Tank!
/// It seems to be a really large project...
/// But don't worry, we do not intend to make it difficult.
/// Enjoy this homework and good luck, OwO!
///
/// Getting started:
/// 1. Compile with `gcc Tank/Main.c`.
/// 2. Run the executable.
/// 3. Move with "WASD" and shoot with "K".
/// 4. Exit with "ESC".

//
//
//
//
//
#include "Game.h"

int main(void) {
  // Uses the current time as a seed for the pseudo-random number generator.
  srand(time(NULL));

  // Configure the game.
  config.fps = 30;
  config.mapSize = (Vec){20, 20};
  config.nEnemies = 0;
  config.nSolids = 0;
  config.nWalls = 0;
  config.PlayerMoveCooldown = (int)(config.fps / 5.0);
  config.EnemyMoveCooldown = (int)(config.fps / 3.0);
  config.PlayerShootCooldown = (int)(config.fps / 10.0);
  config.EnemyShootCooldown = (int)(config.fps / 6.0);
  config.BulletMoveCooldown = (int)(config.fps / 15.0);

  // Run the game.
  GameLifecycle();

  return 0;
}
