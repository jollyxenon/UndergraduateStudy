#include "pvz/GameGeometry/GridGeometry.hpp"

#include "pvz/utils.hpp"

// A point belongs to the lawn when it lies within the rectangular grid bounds.
bool IsInsideLawnGrid(int x, int y) {
  return x >= LAWN_GRID_LEFT && x < LAWN_GRID_RIGHT && y >= LAWN_GRID_BOTTOM &&
         y < LAWN_GRID_TOP;
}

// Horizontal grid lookup divides the offset from the lawn's left boundary.
int GetGridColFromX(int x) { return (x - LAWN_GRID_LEFT) / LAWN_GRID_WIDTH; }

// Vertical grid lookup uses the top boundary because row zero is visually top.
int GetGridRowFromY(int y) { return (LAWN_GRID_TOP - y) / LAWN_GRID_HEIGHT; }

// Cell centers sit half a cell width after the column's left edge.
int GetGridCenterX(int col) {
  return LAWN_GRID_LEFT + col * LAWN_GRID_WIDTH + LAWN_GRID_WIDTH / 2;
}

// Cell centers sit half a cell height below the row's top edge.
int GetGridCenterY(int row) {
  return LAWN_GRID_TOP - row * LAWN_GRID_HEIGHT - LAWN_GRID_HEIGHT / 2;
}
