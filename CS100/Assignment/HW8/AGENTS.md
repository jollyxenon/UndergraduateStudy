# CS100 HW8 Agent Instructions

This file is scoped to `CS100/Assignment/HW8`. The editable C++ project is in `attachment/`, not in this directory root.

## Project layout

- Assignment PDFs (`1_概览.pdf`, `2_游戏攻略.pdf`, `3_评分标准.pdf`, `4_FAQ.pdf`) live at the HW8 root and are Git LFS binaries.
- `attachment/` is the only source project: `src/`, `include/`, `assets/`, `third_party/`, `CMakeLists.txt`, and submit scripts.
- `example/` contains reference executables for Windows/macOS; do not treat it as source.
- `TODO.md` records the current implementation checklist and known environment issue.

## Build, run, submit

- Work from `attachment/` for all build and source edits.
- Configure/build: `cmake -B build -S .` then `cmake --build build`.
- Executable path after build: `build/bin/PvZ`.
- Submission package: run `bash submit.sh` from `attachment/`; it creates `submission.zip` containing only `src/`, `include/`, and `CMakeLists.txt`. On Windows, use `submit.bat` from the same directory.
- If using the configured pixi environment, `TODO.md` notes that `pixi run cmake -B build -S .` has failed before due to missing OpenGL libraries (`OPENGL_opengl_LIBRARY`, `OPENGL_glx_LIBRARY`, `OPENGL_INCLUDE_DIR`).

## Code organization

- C++ standard is C++17. The root `attachment/CMakeLists.txt` enables `-Wall -Wextra -Wpedantic -Wno-unused-variable` for C++ on non-MSVC.
- Main editable modules are currently `src/GameObject/`, `include/pvz/GameObject/`, `src/GameWorld/`, and `include/pvz/GameWorld/`.
- If adding a new source directory, add its own `CMakeLists.txt` and wire it through the parent `CMakeLists.txt` with `add_subdirectory`.
- Assignment requirements in `TODO.md` expect at least one new `.hpp` and one new `.cpp`, standard containers/smart pointers, `enum class`/existing `utils.hpp` enums, private members where practical, constructor initializer lists, and comments for each function.
- Avoid `dynamic_cast`, `typeid`, or `ImageID` checks to distinguish game object types; prefer virtual functions or explicit category/state data.

## File handling gotchas

- Do not commit build outputs, `submission.zip`, or generated executables unless explicitly requested.
- Assets and PDFs are binary/LFS-managed; use appropriate PDF/image tools instead of plain text reads.

## Code standard

- After each batch modification, use 'pixi run format' to format the code.
- Add normative comments to all structures, classes, functions, and long procedures.
