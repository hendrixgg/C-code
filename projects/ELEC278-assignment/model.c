#include "model.h"     // Header file that this file implements.
#include "interface.h" // for update_cell_display and CELL_DISPLAY_WIDTH.
#include "stack.h"     // for stack_t, stack_new, stack_push, stack_pop, and stack_free.

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
            // Set the row and column of the cell.
            table[row][col].r = row;
            table[row][col].c = col;
            // Inialize the input type as CONST.
            table[row][col].input_type = CONST;
            // Initialize the data type as STRING.
            table[row][col].data_type = STRING;
            // Initalize the text_input string to "".
            table[row][col].text_input = strdup("");
            table[row][col].text_input_len = 0;
            // Initialize the text_display string to empty.
            table[row][col].text_display[0] = '\0';
            // The children and parents array starts out as empty.
            table[row][col].num_children = table[row][col].num_parents = 0;
            // Set the flags to false.
            table[row][col].in_process = table[row][col].on_stack = false;
            table[row][col].children_processed = 0;
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
    // assert assumptions.
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

/**
 * Updates `c1->text_display` to contain `c1->num` with 1 digit after the decimal point.
 *
 * @param cell The pointer to cell whose text_display to update. Must be a cell with data_type `NUMBER`, otherwise undefined behaviour.
 */
void cell_update_display_num(cell_t *c1)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(c1->data_type == NUMBER);

    // Set the display text to have 1 digit after a decimal point.
    snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "%.1f", c1->num);
}

/**
 * Checks if the character is an operator. Currently only supporting '+' and '-'.
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
 * Updates the c1->num and c1->text_display by re-evaluating the formula stored in the cell's text_input. Will update the parent-child relationships if new_formula is true.
 *
 * @param cell The cell whose formula to update. Must be a cell with input_type FORMULA, otherwise undefined behaviour.
 * @param new_formula A flag to indicate if the formula is new. If true, then c1->parents must have been cleared before calling this function and that c1->num_parents == 0. New parent-child relationships will be created for each cell referenced in the formula. Otherwise, the existing parent-child relationships will remain unchanged.
 *
 * @return true if the formula was updated successfully, otherwise false.
 */
bool cell_update_formula(cell_t *c1, bool new_formula)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(c1->text_input != NULL);
    assert(c1->text_input[0] == '=');
    assert(c1->input_type == FORMULA);
    assert((new_formula && c1->num_parents == 0) || !new_formula);

    // Pointer to the beginning of the formula string (after the equals sign).
    char *text = c1->text_input + 1;
    // The previous operator. Assuming that we are starting by doing 0+...
    char prev_op = '+';
    // Start with a result of 0.
    number result = 0;
    // Could support more complex expressions by first converting the expression to postfix notation taking into account brackets and operator precedence, and then evaluating the postfix expression.
    // Loop over the formula string.
    for (number tmp_num; text != NULL && is_operator(prev_op); prev_op = *(text++))
    {
        // Got a number. Parse it.
        if (isdigit(*text))
        {
            // Was not a valid number.
            if (!parse_number(text, &text, &tmp_num))
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:NAN");
                c1->data_type = ERROR;
                return false;
            }
        }
        // Got a letter. Check if it is a valid cell reference.
        else if (isalpha(*text))
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
            // row number or column invalid.
            if (endptr == text || errno == ERANGE || (parent_row < ROW_1 || parent_row > NUM_ROWS) || (parent_col < COL_A || parent_col > NUM_COLS))
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:REF");
                c1->data_type = ERROR;
                return false;
            }
            // Not checking for self reference circular dependency. Circular dependencies will be caught when updating children.
            cell_t *p = &table[parent_row][parent_col];
            // If we are dealing with a new formula, then update to reflect new parent.
            // If this is a repeated reference to the same parent, this code will add a duplicate parent of p in c1->parents and a duplicate child of c1 in p->children. This is not a breaking problem since we are duplicating in both directions here and deleting in both directions when set_cell_value is called. This inefficiency could be avoided by storing a flag on the parent cell for the duration of this function call to indicate that it has already been added as a parent of c1, then only adding a parent if that flag is false, then setting the flag to true once added as a parent. Finally, loop over parents of c1 and reset the flags to false before returning from the function. There is a trade-off between memory and speed here. If the same reference is included multiple times in the formula expression the current implementation is simpler but uses more memory when adding duplicate parents and incurs a slowdown when removing the duplicates in set_cell_value. The alternative implementation would likely end up using less memory overall (only storing an additional boolean flag compared to storing multiple duplicate pointers in the parents array) and would be slower when looping over parents to clear the flags at the end of the function call but would be faster when clearing parents in set_cell_value.
            if (new_formula)
            {
                // Add new parent to parents array of c1.
                c1->parents[c1->num_parents++] = (cell_pos_t){p, p->num_children};
                // Add c1 to children array of parent.
                p->children[p->num_children++] = (cell_pos_t){c1, c1->num_parents - 1};
            }
            // Ensure this p is has data type NUMBER before continuing with the rest of the evaluation.
            if (p->data_type != NUMBER)
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:REFNAN");
                c1->data_type = ERROR;
                return false;
            }
            // Advance the text position to the end of the row number.
            text = endptr;
            // Set the tmp_num to the parent cell's number.
            tmp_num = p->num;
        }
        else
        {
            // Invalid character.
            snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:SYNTAX");
            c1->data_type = ERROR;
            return false;
        }
        // Overflow and underflow is not being checked.
        switch (prev_op)
        {
        case '+':
            result += tmp_num;
            break;
        case '-':
            result -= tmp_num;
            break;
        // Should never reach default since we are checking for valid operators in the loop condition.
        default:
            assert(false);
            break;
        }
    }
    // If the whole input was read correctly, then update the cell.text_display.
    if (prev_op == '\0')
    {
        c1->num = result;
        c1->data_type = NUMBER;
        cell_update_display_num(c1);
        return true;
    }
    // Otherwise, there was a syntax error.
    snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:SYNTAX");
    c1->data_type = ERROR;
    return false;
}

// all cells should have set in_progress and on_stack flags to be false before calling this function from outside.
// stack starts empty.
// dfs from c1:
//      if cell has already been added to the stack, then we can return true.
//      Flag the current cell as in_process.
//      loop over children:
//          if a child is flagged as in_process. - Circular dependency. don't dfs on this child. skip to next child.
//          call dfs on child.
//
//      unflag the current cell as being processed.
//      add the current cell to the stack. mark it as on_stack.
void dfs_update_ordering(cell_t *c1, stack_t *s)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(s != NULL);
    assert(!c1->in_process);
    assert(s != NULL);

    // c1 is already on the stack. Return.
    if (c1->on_stack)
        return;
    // Flag the current cell as being processed. This is the only place where we set in_process to true.
    c1->in_process = true;
    // Process each child.
    for (c1->children_processed = 0; c1->children_processed < c1->num_children; c1->children_processed++)
    {
        assert(c1->children[c1->children_processed].cell->input_type == FORMULA);
        // If a child is already in process, we know that must be circularly referring to c1.
        if (c1->children[c1->children_processed].cell->in_process)
        {
            snprintf(c1->children[c1->children_processed].cell->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:CIRC");
            c1->children[c1->children_processed].cell->data_type = ERROR;
            c1->children[c1->children_processed].cell->circular_dependency = true;
            // TODO: Could keep track that this parent-child relationship caused errors so we can tell the user. For now just mark the child as an ERR:CIRC.
            // c1 is the circular parent of c1->children[c1->children_processed].cell
            continue;
        }
        // Child cell is not causing a circular dependency. dfs on it.
        c1->children[c1->children_processed].cell->circular_dependency = false;
        // Process the child.
        dfs_update_ordering(c1->children[c1->children_processed].cell, s);
    }
    // Unflag the current cell as being processed.
    c1->in_process = false;
    // Add the current cell to the stack since all chlidren have been processed.
    stack_push(s, c1);
    c1->on_stack = true;
}

void set_cell_value(ROW row, COL col, char *text)
{
    // assert assumptions.
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

    // Reset c1's parents.
    // Remove c1 from all its parents' children arrays.
    // Do this by moving the last child in each parent array into the position of the current cell, and decrementing the parent's num_children.
    // Then update the moved child's saved position in the parent's children array.

    // i = c1_parents_indexOf_p.
    for (size_t i = 0; i < c1->num_parents; i++)
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
    // Set num_parents to zero since we removed c1 from all of their children arrays.
    c1->num_parents = 0;

    // Update the cell with it's new text_input.
    // Save the old numeric value and type in case we need to update children.
    // FORMULA
    if (*(c1->text_input) == '=')
    {
        c1->input_type = FORMULA;
        cell_update_formula(c1, true);
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
        else
        {
            c1->data_type = STRING;
            snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, c1->text_input);
        }
    }

    // TODO: If the cell has children, perform updates.
    if (c1->num_children > 0)
    {
        // stack to store pointers to cells in the order that they should be updated.
        stack_t *s = stack_new(sizeof(cell_t *));
        // This will result in the cells with no children (farthest descendents of c1) being at the bottom of the stack, and the cells with the most children (direct descendents of c1 and c1 itself) being at the top of the stack.
        dfs_update_ordering(c1, s);
        // This also flags any cells that are causing a circular dependency.
        // Pointer for cell popped off the stack.
        cell_t *out;
        // loop while the stack is not empty.
        // Pop of each cell from the stack and update it.
        // This ordering is such that as each cell is popped from the stack, all of it's parents have been updated already.
        while (stack_pop(s, &out))
        {
            out->on_stack = false;
            // If there was no circular dependency, then update the cell by recalculating the formula.
            if (!out->circular_dependency && out->input_type == FORMULA)
                cell_update_formula(out, false);
            // Update the display text in the interface.
            update_cell_display(out->r, out->c, out->text_display);
        }
        // Free the stack.
        stack_free(s);
    }
    // Update display text in the interface.
    else
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
