# OpenCode Agent Instructions

This repository contains personal undergraduate study materials for a student at ShanghaiTech University.

## Repository Structure & Navigation
- **Course-based Organization**: The root directory contains folders for each course (e.g., `CS100`, `MATH1112`, `GEHA1158`).
- **Standard Subdirectories**: Inside each course folder, you will typically find:
  - `Assignment/` or `HW/`: Homework and programming assignments.
  - `Courseware/`: Lecture slides and materials.
  - `Note/`: Personal study notes.
  - `Exam/`, `Midterm/`, `Final/`: Exam preparation materials.
- **Workflow**: When asked to work on a specific task, always navigate to the relevant course and subdirectory first. There is no global build system or monorepo toolchain.

## Course-Specific Context
- **CS100 (Computer Science)**: Contains C programming assignments. Compile files locally within their specific assignment folder (e.g., `gcc Problem2.c -o problem2.exe`).
- **Humanities/Social Sciences (GEHA*, GESS*)**: Contains essays and reading materials, often in Chinese. Expect `.docx` and `.txt` files.
- **Math/Science (MATH*, CHEM*, SI*)**: Primarily contains `.pdf` files for homework, answers, and courseware.

## Technical Quirks & Constraints
- **Git LFS**: The repository uses Git LFS for binary files (`.pdf`, `.docx`, `.pptx`, `.xlsx`, `.png`, `.mp4`, etc.). Do not attempt to read these files as plain text.
- **Windows Environment**: The repository is developed on Windows (note the `.exe` files in `CS100` and `__pycache__` in `.gitignore`). Use appropriate Windows/PowerShell commands when executing scripts or compiled programs.
- **Grades**: The `README.md` at the root tracks the student's grades for each semester. Update it if requested when a new course is completed.