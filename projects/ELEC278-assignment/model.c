#include "model.h"
#include "interface.h"

// TODO: Comment on what is used from each standard library header file.
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

// Classification of user input to a cell.
typedef enum _CELL_TYPE
{
    EMPTY,
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
    // Stores which type of input is in the cell is. Either EMPTY, CONST, or FORMULA.
    CELL_TYPE input_type;
    // Stores the data type of the cell. Either a NUMBER, STRING, or ERROR. Undefined for input_type of EMPTY.
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
            // Inialize the data type as EMPTY.
            table[row][col].input_type = EMPTY;
            // Initalize the text_input string.
            table[row][col].text_input = malloc(sizeof(char));
            table[row][col].text_input[0] = '\0';
            table[row][col].text_input_len = 0;
            // Initialize the text_display string to empty.
            table[row][col].text_display[0] = '\0';
            // The children and parents array starts out as empty.
            table[row][col].num_children = table[row][col].num_parents = 0;
            // // Set the depth and depth_flag to 0.
            // table[row][col].depth_flag = table[row][col].depth = 0;
            // Set the flag to false.
            table[row][col].flag = false;
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
    // Set the endptr to text so that we can check if it was updated when calling strtof.
    char *endptr_temp = (char *)text;
    if (endptr == NULL)
        endptr = &endptr_temp;
    // Set errno to zero so we can check if it was updated when calling strtof.
    errno = 0;
    // Parse a float. This algorithm supoprts scientific notation.
    *out = strtof(text, endptr);
    // return whether the parsing was successful.
    return *endptr != text && errno != ERANGE && !isinf(*out) && !isnan(*out);
}

void cell_update_display_num(cell_t *c1)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(c1->input_type == CONST || c1->input_type == FORMULA);

    // Set the display text to have 1 digit after a decimal point.
    snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "%.1f", c1->num);
}

/**
 * Checks if the character is an operator. Currently only supports '+' and '-'.
 *
 * @param op The character to check.
 *
 * @return true if the character is an operator, otherwise false.
 */
bool is_operator(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return true;
    default:
        return false;
    }
}

/**
 * Updates the cell.num and cell.text_display by re-evaluating the formula stored in the cell's text_input.
 *
 * @param cell The cell whose formula to update.
 *
 * @return true if the formula was updated successfully, otherwise false.
 */
bool cell_update_formula(cell_t *c1)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(c1->text_input != NULL);
    assert(c1->text_input[0] == '=');
    assert(c1->input_type == FORMULA);

    // Pointer to the beginning of the formula string (after the equals sign).
    char *text = c1->text_input + 1;
    // The previous operator. Assuming that we are starting by doing 0+...
    char prev_op = '+';
    // reset the cell.num to 0.
    c1->num = 0;
    // Loop over the formula string.
    for (number tmp_num; text != NULL && is_operator(prev_op); prev_op = *(text++))
    {
        // Identified a potential cell reference.
        if (*text >= 'A' && *text <= 'G')
        {
            // Convert the letter to a column.
            COL parent_col = *text - 'A';
            // Next character. Check if it is a valid row number.
            assert(++text != NULL);
            // Set the endptr to text so that we can check if it was updated when calling strtol.
            char *endptr = (char *)text;
            // Parse an int.
            errno = 0;
            ROW parent_row = (ROW)(strtol(text, &endptr, 10) - 1);
            // Was not a valid row number.
            if (endptr == text || errno == ERANGE || (parent_row < ROW_1 || parent_row > NUM_ROWS))
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:SYNTAX");
                c1->data_type = ERROR;
                return false;
            }
            cell_t *p = &table[parent_row][parent_col];
            // Check for circular dependency. Ensure not referring to self. Reference to a child/grandchild cell is checked for when updating children. Self references are also checked then, but we can check here to avoid the error down the line
            if (p == c1)
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:CIRC");
                c1->data_type = ERROR;
                return false;
            }
            // Ensure this cell is a (FORMULA or CONST) and data type NUMBER.
            if (!((p->input_type == FORMULA || p->input_type == CONST) && p->data_type == NUMBER))
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:TYPE");
                c1->data_type = ERROR;
                return false;
            }
            // Advance the text position to the end of the row number.
            text = endptr;
            // set the tmp_num to the parent cell's number.
            tmp_num = p->num;
            // Add new parent to parents array of c1.
            c1->parents[c1->num_parents++] = (cell_pos_t){p, p->num_children};
            // update the children array of the parent cell.
            p->children[p->num_children++] = (cell_pos_t){c1, c1->num_parents - 1};
        }
        // Must be a number. If parsing fails, return false.
        else if (!parse_number(text, &text, &tmp_num))
        {
            snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:NAN");
            c1->data_type = ERROR;
            return false;
        }
        assert(text != NULL);
        // Overflow and Underflow is not being checked.
        switch (prev_op)
        {
        case '+':
            c1->num += tmp_num;
            break;
        case '-':
            c1->num -= tmp_num;
            break;
        // Should never reach here since we are checking for operators in the for loop condition.
        default:
            assert(false);
            break;
        }
    }
    // If the whole input was read correctly, then update the cell.text_display.
    if (prev_op == '\0')
    {
        cell_update_display_num(c1);
        c1->data_type = NUMBER;
        return true;
    }
    snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:SYNTAX");
    c1->data_type = ERROR;
    return false;
}

void set_cell_value(ROW row, COL col, char *text)
{
    assert(text != NULL);
    assert(row >= ROW_1 && row < NUM_ROWS);
    assert(col >= COL_A && col < NUM_COLS);

    // Pointer to the current cell.
    cell_t *c1 = &table[row][col];

    // Clear the old the input text.
    free(c1->text_input);
    // Set the new input text.
    c1->text_input = text;
    c1->text_input_len = strlen(text);

    // Remove c1 from all its previous parents' children arrays.
    // Do this by moving the last child in each parent array into the position of the current cell, and decrementing the parent's num_children.
    // Then update the moved child's saved position in the parent's children array.

    // c1_parents_indexOf_p.
    size_t i;
    for (i = 0; i < c1->num_parents; i++)
    {
        // Parent cell that c1 is being removed from.
        cell_t *p = c1->parents[i].cell;
        // Position of c1 in p->children.
        size_t p_children_indexOf_c1 = c1->parents[i].pos;
        // We know that p->num_children > 0 since c1 was in p->children. So we can safely decrement p->num_children.
        // If c1 was the last child in the array, then there is nothing to swap.
        if (--p->num_children == 0)
            continue;
        // Sibling cell that is being moved to the current cell's position. (last child in p->children)
        cell_t *c2 = p->children[p->num_children].cell;
        // c2_parents_indexOf_p.
        size_t j = p->children[p->num_children].pos;
        // Overwrite record of c1 with c2.
        p->children[p_children_indexOf_c1] = p->children[p->num_children];
        // Update position of p in c2->parents.
        c2->parents[j].pos = p_children_indexOf_c1;
    }
    // Set parents to zero since we removed c1 from all of their children arrays.
    c1->num_parents = 0;

    // Update the cell with it's new text_input.
    // Save the old numeric value and type in case we need to update children.
    number old_num = c1->num;
    CELL_TYPE old_input_type = c1->input_type;
    DATA_TYPE old_data_type = c1->data_type;
    // FORMULA
    if (*(c1->text_input) == '=')
    {
        c1->input_type = FORMULA;
        cell_update_formula(c1);
    }
    // CONST
    else
    {
        c1->input_type = CONST;
        // NUMBER
        if (isdigit((*(c1->text_input))) && parse_number(c1->text_input, NULL, &c1->num))
        {
            c1->data_type = NUMBER;
            cell_update_display_num(c1);
        }
        // STRING
        else if (c1->text_input_len > 0)
        {
            c1->data_type = STRING;
            snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, c1->text_input);
        }
    }

    // TODO: If the cell has changed and has children, update them.
    if (c1->num_children > 0 && (old_input_type != c1->input_type || old_data_type != c1->data_type || old_num != c1->num))
    {

        // all cells should be flagged as not visited.
        // all cells should be flagged as not on the stack.
        // stack starts empty.
        // dfs from c1:
        //      if cell has already been added to the stack, then we can return true.
        //      if the cell is flagged as being processed, then there is a circular dependency. return false.
        //      Flag the current cell as being processed.
        //      loop over children, call dfs on each child:
        //          if a child return false exit the loop and return false. alternatively, could keep track of which children caused errors so we can display them to the user.
        //      unflag the current cell as being processed.
        //      if there were no circular dependencies, add the current cell to the stack. and mark current cell as on the stack. return true.
        //      if there were circular dependencies, return false.
        // if dfs returns false, then there was a circular dependency. set c1 to ERR:CIRC (or set the error to show which cells it is circularly referring to), clear the stack, and return.

        // c1 was on the stack, so we can continue and update the children. start by popping c1 off the stack. since we updated it earlier on in this function, we don't need to update it again.
        // This will result in the cells with no children being at the bottom of the stack, and the cells with the most children (direct descendents of c1) being at the top of the stack.

        // loop while the stack is not empty.
        // Pop of each cell from the stack and update it.
        // This will result in such that as each cell is popped from the stack, all of it's parents have been popped already.
    }
    // Update display text in the interface.
    update_cell_display(row, col, c1->text_display);
}

void clear_cell(ROW row, COL col)
{
    // assert assumptions.
    assert(row >= ROW_1 && row < NUM_ROWS);
    assert(col >= COL_A && col < NUM_COLS);

    // Call set cell value with an empty string. This handles all child/parent updates and updating the display text in the interface.
    set_cell_value(row, col, strdup(""));
}

char *get_textual_value(ROW row, COL col)
{
    return strdup(table[row][col].text_input);
}
