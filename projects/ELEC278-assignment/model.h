#ifndef ASSIGNMENT_MODEL_H
#define ASSIGNMENT_MODEL_H

#include "defs.h"

/**
 * Initializes the spreadsheet data structure.
 *
 * This is called once, at program start.
 */
void model_init();

/**
 * Sets the value of a cell based on user input. Updates the interface to reflect the new value by calling 'update_cell_display' (see interface.h).
 *
 * @param row The row of the cell.
 * @param col The column of the cell.
 * @param text The new value of the cell, as a string. This string is now owned by this function and/or the cell contents data structure; it is its responsibility to ensure it is freed once it is no longer needed.
 */
void set_cell_value(ROW row, COL col, char *text);

/**
 * Clears the value of a cell. The resulting display text should be an empty string.
 *
 * @param row The row of the cell.
 * @param col The column of the cell.
 */
void clear_cell(ROW row, COL col);

/**
 * Gets a textual representation of the value of a cell, for editing.
 *
 * @param row The row of the cell.
 * @param col The column of the cell.
 *
 * @return A textual representation of the value of the cell, or NULL if the cell is empty. The returned string must have been allocated using 'malloc' and is now owned by the interface. The cell contents data structure must not modify it or retain any reference to it after the function returns. The returned string must be freed by the interface when it is no longer needed.
 *
 */
char *get_textual_value(ROW row, COL col);

#endif // ASSIGNMENT_MODEL_H
