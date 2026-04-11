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
  config.fps = 60;
  config.mapSize = (Vec){Rand(10) + 40, Rand(5) + 25};
  config.nEnemies = Rand(3) + 1;
  config.nSolids = Rand(5) + 4;
  config.nWalls = Rand(5) + 4;
  config.PlayerMoveCooldown = (int)(config.fps * 0.2);
  config.EnemyMoveCooldown = (int)(config.fps * 0.5);
  config.PlayerShootCooldown = (int)(config.fps * 0.5);
  config.EnemyShootCooldown = (int)(config.fps * 1.0);
  config.BulletMoveCooldown = (int)(config.fps * 0.1);

  // Run the game.
  GameLifecycle();

  return 0;
}
