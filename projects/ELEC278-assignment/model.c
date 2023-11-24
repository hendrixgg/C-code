#include "model.h"
#include "interface.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

typedef enum _CELL_TYPE
{
    NUMBER,
    STRING,
    FORMULA,
    ERROR,
} CELL_TYPE;

// v = value union, type = type of value.
typedef struct _number
{
    union number_union
    {
        int i;
        float f;
    } v;
    enum NUMBER_TYPE
    {
        INT,
        FLOAT,
    } type;
} number;

typedef struct _cell_pos_t cell_pos_t;
typedef struct _cell_t cell_t;

// Stores a pointer to a cell, and a size_t pos.
struct _cell_pos_t
{
    // Pointer to the cell.
    cell_t *cell;
    // The position of the parent cell in the chlid->parents array.
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
    // Stores which type input the cell is. Either a number, string, or formula.
    CELL_TYPE type;
    // Store the numerical value of the cell. If the cell is a string, then this value is undefined.
    number value;

    // This array will have to be dynamically updated as new parents are added/removed.
    // Array of {cell_t *, size_t} = {parent cell, position of current cell in parent cell children array}.
    cell_pos_t *parents;
    size_t num_parents;

    // This array will have to be dynamically updated as new children are added/removed.
    // Array of {cell_t *, size_t} = {child cell, position of current cell in child cell parent array}.
    cell_pos_t *children;
    size_t num_children;

    // A flag used to mark the longest path to the cell during an update. Used to avoid a circular dependency and to ensure that cells are updated in the correct order.
    size_t depth_flag;
    // The longest path to a parent/grandparent of the cell. depth = 0 means that this cell does not refer to other cells. Calculated when evaluating the formula.
    size_t depth;
    // TODO: figure out what is needed here.
    // A flag used to mark if the cell has been visited during an update. Used to avoid a circular dependency and to ensure that cells are updated in the correct order.
    bool flag;
};

// Stores all the cells in the table;
static cell_t table[NUM_ROWS][NUM_COLS];

void model_init()
{
    // For each row in the table.
    for (ROW row = ROW_1; row < NUM_ROWS; row++)
    {
        // For each column in the row.
        for (COL col = COL_A; col < NUM_COLS; col++)
        {
            // Inialize the data type as STRING.
            table[row][col].type = STRING;
            // Initalize the text_input string.
            table[row][col].text_input = malloc(sizeof(char));
            table[row][col].text_input[0] = '\0';
            table[row][col].text_input_len = 0;
            // Initialize the text_display string to empty.
            table[row][col].text_display[0] = table[row][col].text_display[CELL_DISPLAY_WIDTH] = '\0';
            // The children array starts out as empty.
            table[row][col].children = NULL;
            table[row][col].num_children = 0;
            // Set the depth and depth_flag to 0.
            table[row][col].depth_flag = table[row][col].depth = 0;
        }
    }
}

/**
 * Parses the text for a number. If the text is a number, then the number is stored in the 'out' parameter, and true is returned. Otherwise, false is returned.
 * @param text The text to parse for a number.
 * @param out The location to store the number that was parsed.
 * @param endptr A reference to store a pointer to the position in text after the number. If NULL, then this parameter is ignored.
 *
 * @return true if a number was parsed successfully, otherwise false.
 */
bool parse_number(const char *text, char **endptr, number *out)
{
    assert(text != NULL);
    assert(out != NULL);
    // If the user didn't specify an endptr, then use a temporary one.
    char *endptr_temp = NULL;
    if (endptr == NULL)
        endptr = &endptr_temp;
    *endptr = NULL;
    // Parse an int.
    errno = 0;
    out->v.i = (int)strtol(text, endptr, 10);
    // Was a valid int. Retrun INT.
    if (*endptr != NULL && **endptr == '\0' && errno != ERANGE)
    {
        out->type = INT;
        return true;
    }
    // Parse a float.
    errno = 0;
    out->v.f = strtof(text, endptr);
    // Was a valid float. Retrun FLOAT.
    if (*endptr != NULL && **endptr == '\0' && errno != ERANGE && !isinf(out->v.f) && !isnan(out->v.f))
    {
        out->type = FLOAT;
        return true;
    }
    // Was not a number.
    return false;
}

/**
 *
 * @param cell The cell to update the formula for, based on it's text_input.
 * @param curr_depth The current depth of the cell. Used to avoid circular dependencies.
 */
bool update_cell_formula(cell_t *cell, size_t curr_depth)
{
    // assert assumptions.
    assert(cell != NULL);
    assert(cell->type == FORMULA);
    assert(cell->text_input != NULL);
    assert(cell->text_input[0] == '=');

    bool error = false;
    char *text = cell->text_input;
    // Loop over the formula string.
    for (number value; !error && text != NULL && *text != '\0'; text++)
    {
        // Identified a potential cell reference.
        if (*text >= 'A' && *text <= 'G')
        {
            // Convert the letter to a column.
            COL parent_col = *text - 'A';
            // Next character. Check if it is a valid row number.
            assert(++text != NULL);
            // Pointer to the beginning of the input.
            char *endptr = NULL;
            // Parse an int.
            errno = 0;
            ROW parent_row = (ROW)(strtol(text, &endptr, 10) - 1);
            // Was not a valid row number, or was a reference to the current cell. Exit with error.
            if (endptr == NULL || errno == ERANGE || (parent_row < ROW_1 || parent_row > NUM_ROWS) || &table[parent_row][parent_col] == cell)
            {
                strncpy(cell->text_display, "ERR:SYNTAX", CELL_DISPLAY_WIDTH + 1);
                cell->type = ERROR;
                error = true;
                break;
            }
            // Advance the text position to the end of the row number.
            text = endptr;
            value = table[parent_row][parent_col].value;
            // Ensure this cell is a number or formula.
            if (value.type != NUMBER && value.type != FORMULA)
            {
                strncpy(cell->text_display, "ERR:TYPE", CELL_DISPLAY_WIDTH + 1);
                cell->type = ERROR;
                error = true;
                break;
            }
            // Check for circular dependency.
            if (table[parent_row][parent_col].flag)
            {
                strncpy(cell->text_display, "ERR:CIRC", CELL_DISPLAY_WIDTH + 1);
                cell->type = ERROR;
                error = true;
                break;
            }
        }
        // Must be a number
        else if (!parse_number(text, &text, &value))
        {
            strncpy(cell->text_display, "ERR:NAN", CELL_DISPLAY_WIDTH + 1);
            cell->type = ERROR;
            error = true;
            break;
        }
        assert(text != NULL);
        // If we were using INT until now, but now see floats, then convert value type to float.
        if (value.type == FLOAT && cell->value.type == INT)
        {
            cell->value.v.f = value.v.f;
            cell->value.type = FLOAT;
        }
        // TODO: check for an operator
    }
    return !error && text != NULL && *text == '\0';
}

void set_cell_value(ROW row, COL col, char *text)
{
    assert(text != NULL);
    assert(row >= ROW_1 && row < NUM_ROWS);
    assert(col >= COL_A && col < NUM_COLS);

    // Reset the input text.
    free(table[row][col].text_input);
    table[row][col].text_input = text;
    table[row][col].text_input_len = strlen(text);

    // Update the cell with it's new text_input.
    // Save the old value and type in case we need to update children.
    number old_value = table[row][col].value;
    CELL_TYPE old_type = table[row][col].type;
    switch (*(table[row][col].text_input))
    {

    // Formula
    case '=':
        // parse_formula(table[row][col].text_input, row, col);
        table[row][col].type = FORMULA;
        strncpy(table[row][col].text_display, table[row][col].text_input, CELL_DISPLAY_WIDTH + 1);
        break;
    // INT or FLOAT
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        parse_number(table[row][col].text_input, NULL, &table[row][col].value);
        switch (table[row][col].type)
        {
            // TODO: FIX ERROR HERE.
        case INT:
            // Set the display text.
            strncpy(table[row][col].text_display, table[row][col].text_input, CELL_DISPLAY_WIDTH + 1);
            break;
        case FLOAT:
            // Set the display text to have at most 1 digit after a decimal point.
            snprintf(table[row][col].text_display, CELL_DISPLAY_WIDTH + 1, "%.1f", table[row][col].value.v.f);
            break;
        // Not a number. Must be STRING.
        case ERROR:
            table[row][col].type = STRING;
            strncpy(table[row][col].text_display, table[row][col].text_input, CELL_DISPLAY_WIDTH + 1);
            break;
        // Should never happen.
        default:
            assert(false);
            break;
        }
        break;
    // STRING
    default:
        table[row][col].type = STRING;
        strncpy(table[row][col].text_display, table[row][col].text_input, CELL_DISPLAY_WIDTH + 1);
        break;
    }

    // TODO: If the cell has changed and has children, update them.
    // if (table[row][col].type != old_type || table[row][col].value.i != old_value.i)
    // {
    //     // Flag our current cell as visited before updating children.
    //     table[row][col].depth_flag = 0;
    //     // For each child.
    //     for (size_t i = 0; i < table[row][col].num_children; i++)
    //     {
    //         // Update the child cell by recalculating it's value by it's formula.
    //         update_cell_formula(&table[row][col].children[i].cell);
    //         // If there is a circular dependency, then set the display text to "CIR. DEP.".
    //     }
    //     table[row][col].flag = false;
    // }

    // Update display text in the interface.
    update_cell_display(row, col, table[row][col].text_display);

    // switch (table[row][col].type)
    // {
    // case INT:
    //     update_cell_display(row, col, "INT");
    //     break;
    // case FLOAT:
    //     update_cell_display(row, col, "FLOAT");
    //     break;
    // case STRING:
    //     update_cell_display(row, col, "STRING");
    //     break;
    // case ERROR:
    //     update_cell_display(row, col, "ERROR");
    //     break;
    // case FORMULA:
    //     update_cell_display(row, col, "FORMULA");
    //     break;

    // default:
    //     assert(false);
    //     break;
    // }
}

void clear_cell(ROW row, COL col)
{
    // TODO: implement this.

    // Clear the input and display strings.
    table[row][col].text_input[0] = table[row][col].text_display[0] = '\0';
    table[row][col].text_input_len = 0;
    table[row][col].type = STRING;
    // Update the interface.
    update_cell_display(row, col, table[row][col].text_display);
}

char *get_textual_value(ROW row, COL col)
{
    return strdup(table[row][col].text_input);
}
