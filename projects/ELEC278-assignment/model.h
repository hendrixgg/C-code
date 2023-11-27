#ifndef ASSIGNMENT_MODEL_H
#define ASSIGNMENT_MODEL_H

#include "defs.h"      // for ROW, COL, NUM_ROWS, NUM_COLS.
#include "interface.h" // for CELL_DISPLAY_WIDTH.

#include <stdbool.h> // for bool.
#include <stddef.h>  // for size_t.

// Classification of user input to a cell.
typedef enum _CELL_TYPE
{
    CONST,
    FORMULA,
} CELL_TYPE;

// Data type stored in a cell.
typedef enum _DATA_TYPE
{
    NUMBER,
    STRING,
    ERROR,
} DATA_TYPE;

typedef struct _cell_pos_t cell_pos_t;
typedef struct _cell_t cell_t;
typedef float number;

// Stores a pointer to a cell, and a size_t pos.
struct _cell_pos_t
{
    // Pointer to the cell.
    cell_t *cell;
    // Position of the cell in the parent/child array.
    size_t pos;
};

/**
 * A cell is a node in a directed-acyclic-graph. Meaning that each cell can have multiple parents and children, but there are no circular dependencies. A particular cell will never be present in either it's children or one of it's children's children.
 */
struct _cell_t
{
    // Stores the text that you see in the input bar at the top as a null character terminated string.
    char *text_input;
    // Length of 'text_input' string.
    size_t text_input_len;
    // Stores the text that is shown in the cell of the table as a null character terminated string.
    char text_display[CELL_DISPLAY_WIDTH + 1];
    // The row of the cell in the table.
    ROW r;
    // The column of the cell in the table.
    COL c;
    // Stores which type of input is in the cell is. Either CONST, or FORMULA.
    CELL_TYPE input_type;
    // Stores the data type of the cell. Either a NUMBER, STRING, or ERROR.
    DATA_TYPE data_type;
    // Store the numerical value of the cell. If the cell is a string, then this value is undefined.
    number num;

    // TODO: Make parents dynamically allocated.
    // This array will have to be dynamically updated as new parents are added/removed.
    // Array of {`cell_t *`, `size_t`} = {parent cell, position of current cell in parent cell children array}.
    // parents[i] = {p_i*, p_i.children.indexOf(this)}
    cell_pos_t parents[NUM_ROWS * NUM_COLS];
    size_t num_parents;

    // TODO: Make children dynamically allocated.
    // This array will have to be dynamically updated as new children are added/removed.
    // Array of {`cell_t *`, `size_t`} = {child cell, position of current cell in child cell parent array}.
    // children[i] = {c_i*, c_i.parents.indexOf(this)}
    cell_pos_t children[NUM_ROWS * NUM_COLS];
    size_t num_children;

    // A flag used to mark if the cell is currently in process for an update. Used to avoid a circular dependency.
    bool in_process;
    // A flag use to mark if the cell and all of it's children have been added to the stack.
    bool on_stack;
    // A how many of this cell's direct children have been processed before this cell can be processed. Used for post-order depth first search.
    size_t children_processed;
    // A flag used to mark if the cell formula is causing a circular dependency. Will not update a cell formula if this is true.
    bool circular_dependency;
};

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
