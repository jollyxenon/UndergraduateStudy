# OpenCode Agents Instructions

This repository contains coursework for various undergraduate courses. 

## CS100 / Assignment / HW4 (Tank 1990)

### Build & Run
- **Compile**: `gcc Tank/Main.c -Wall -Wextra -Wpedantic -Wno-unused-variable`
- **Run**: Execute the compiled binary (e.g., `.\a.exe` on Windows).
- **Format**: Use the provided `.clang-format` file.

### Architecture & Boundaries
- **`Tank/Main.c`**: Entry point. Do not remove `srand(time(NULL));` for final submission.
- **`Tank/Game.h`**: Core game logic. Modify `GameUpdate()` for movement, collision, and shooting.
- **`Tank/Renderer.h`**: Rendering logic. Modify `RdrRender()` to draw tanks and bullets.
- **`Tank/Scene.h`**: Defines game objects (`Tank`, `Bullet`, `Map`) and global registries (`regTank`, `regBullet`).
- **`Tank/Registry.h`**: A custom macro-based ECS-like registry system. **Do not modify this file.**

### Framework Quirks & Conventions
- **Registry Iteration**: You must use the exact macro pattern to iterate over objects:
  ```c
  for (RegIterator it = RegBegin(regTank); it != RegEnd(regTank); it = RegNext(it)) {
      Tank *tank = RegEntry(regTank, it);
      // ...
  }
  ```
- **Memory Management**: Use `RegNew(reg)` to allocate and register an object. Use `RegDelete(ptr)` to remove and free it.
- **Map Access**: The map is a 1D array. Always use `Idx(pos)` to access it with a 2D `Vec`: `map.flags[Idx(pos)]`.
- **Rendering**: 
  - `RdrClear()` clears the previous frame's dynamic objects.
  - `RdrRender()` draws the current frame's dynamic objects using `RdrPutChar(pos, char, color)`.
  - `RdrFlush()` flushes changes to the terminal.
- **Randomness**: Use `Rand(n)` and `RandVec(v)` from `Tank/Base.h`.
