#ifndef GRIDGEOMETRY_HPP__
#define GRIDGEOMETRY_HPP__

// Returns whether a screen point is inside the playable lawn grid rectangle.
bool IsInsideLawnGrid(int x, int y);

// Converts a screen x-coordinate to the containing lawn grid column.
int GetGridColFromX(int x);

// Converts a screen y-coordinate to the containing lawn grid row.
int GetGridRowFromY(int y);

// Converts a lawn grid column to the center x-coordinate of that cell.
int GetGridCenterX(int col);

// Converts a lawn grid row to the center y-coordinate of that cell.
int GetGridCenterY(int row);

#endif  // !GRIDGEOMETRY_HPP__
