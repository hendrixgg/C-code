#ifndef ASSIGNMENT_INTERFACE_H
#define ASSIGNMENT_INTERFACE_H

#include "defs.h"

#define CELL_DISPLAY_WIDTH 11

/**
 * Updates the text which is displayed in a cell.
 * @param row the row of the cell to update.
 * @param col the column of the cell to update.
 * @param text the text to display in the cell. The first CELL_DISPLAY_WIDTH characters will be used, unless the null terminator is reached first. Ensure that `strlen(text) >= CELL_DISPLAY_WIDTH` otherwise there may be memory access errors.
 */
void update_cell_display(ROW row, COL col, const char *text);

#endif // ASSIGNMENT_INTERFACE_H
