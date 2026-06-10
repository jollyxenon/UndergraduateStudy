#ifndef WORLDBASE_H__
#define WORLDBASE_H__

#include <iostream>
#include <memory>
#include <set>

#include "pvz/utils.hpp"

class WorldBase {
 public:
  WorldBase() = default;

  virtual ~WorldBase() = default;

  virtual void Init() = 0;

  virtual LevelStatus Update() = 0;

  virtual void CleanUp() = 0;

  // Runs before ObjectBase dispatches a mouse-down event to clicked objects.
  virtual void BeginMouseDown(int, int) {}
};

#endif  // !WORLDBASE_H__
