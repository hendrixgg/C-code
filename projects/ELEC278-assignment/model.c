#include "model.h"     // Header file that this file implements.
#include "interface.h" // for update_cell_display and CELL_DISPLAY_WIDTH.
#include "stack.h"     // for stack_t, stack_new, stack_push, stack_pop, and stack_free.

#include <stdlib.h> // for strtof and strtol.
#include <string.h> // for size_t, NULL, strlen, strdup.
#include <ctype.h>  // for isdigit and isalpha.
#include <errno.h>  // for errno and ERANGE.
#include <math.h>   // for isnan.
#include <stdio.h>  // for snprintf.
#include <assert.h> // for assert.

// Stores all the cells in the spreadsheet.
static cell_t table[NUM_ROWS][NUM_COLS];

/**
 * Parses the text for a number. If the text is a number, then the number is stored in the 'out' parameter, and true is returned. Otherwise, false is returned.
 *
 * Time Complexity: O(n) where n is the number of characters in the text that make up a valid number.
 *
 * @param text The text to parse for a number. If the text is NULL or invalid, then undefined behaviour occurs.
 * @param out The reference to store the number that was parsed. If the address is NULL or invalid, then undefined behaviour occurs.
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
    return *endptr != text && errno != ERANGE && !isnan(*out);
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
 * Updates the c1->num and c1->text_display by re-evaluating the formula stored in c1->text_input. Optionally, updates the parent-child relationships if the input is new.
 *
 * Time Complexity: O(n) where n is the number of characters in the formula if new_input is false. if new_input is true, then the time complexity is O(n + m) where m is the number of parents c1 previously had.
 *
 * @param cell The cell to be computed based on it's text_input. if `cell` or `cell->text_input` is NULL, then undefined behaviour occurs.
 * @param new_input A flag to indicate if the input is new. If true, then `c1->parents` will be cleared and new parent-child relationships will be created for each cell referenced in the formula. Otherwise, the existing parent-child relationships remain unchanged.
 */
void update_cell(cell_t *c1, bool new_input)
{
    // assert assumptions.
    assert(c1 != NULL);
    assert(c1->text_input != NULL);

    // Reset c1's parents.
    // Remove c1 from each of its parents' children arrays.
    // Do this by moving the last child in each parent array into the position of the current cell, and decrementing the parent's num_children.
    // Then update the moved child's saved position in the parent's children array.
    if (new_input)
    {
        // i = c1_parents_indexOf_p.
        for (size_t i = 0; i < c1->num_parents; i++)
        {
            // Parent cell that c1 is being removed from.
            cell_t *p = c1->parents[i].cell;
            assert(p != NULL);
            // Position of c1 in p->children.
            size_t p_children_indexOf_c1 = c1->parents[i].pos;
            // We know that p->num_children > 0 since c1 was in p->children. So we can safely decrement p->num_children.
            // If c1 was the last child in the array, then there is nothing to swap.
            if (--p->num_children == 0)
                continue;
            // Sibling cell that is being moved to the current cell's position. (last child in p->children)
            cell_t *c2 = p->children[p->num_children].cell;
            assert(c2 != NULL);
            // c2_parents_indexOf_p.
            size_t j = p->children[p->num_children].pos;
            // Overwrite record of c1 with c2.
            p->children[p_children_indexOf_c1] = p->children[p->num_children];
            // Update position of p in c2->parents.
            c2->parents[j].pos = p_children_indexOf_c1;
        }
        // Set num_parents to zero since we removed c1 from all of its parent's children arrays.
        c1->num_parents = 0;
    }

    // Handle a number or string.
    if (c1->text_input[0] != '=')
    {
        // if the input was the same as before, then we don't need to do anything since the cell is already up to date.
        if (!new_input)
            return;
        // NUMBER
        // Was not a valid number or there were other characters after the number
        char *endptr = c1->text_input;
        if (isdigit((*(c1->text_input))) && parse_number(c1->text_input, &endptr, &c1->num) && endptr != c1->text_input && *endptr == '\0')
        {
            c1->data_type = NUMBER;
            cell_update_display_num(c1);
        }
        // STRING
        else
        {
            c1->data_type = STRING;
            snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "%s", c1->text_input);
        }
        return;
    }

    // Handle a formula.
    // If there was an error reading the formula, then return without setting the display text to a number.
    bool error = false;
    // Pointer to the beginning of the formula string (after the equals sign).
    char *text = c1->text_input + 1;
    // The previous operator. Assuming that we are starting by doing 0+...
    char prev_op = '+';
    // Start with a result of 0.
    number result = 0;
    // Could support more complex expressions by first converting the expression to postfix notation taking into account brackets and operator precedence, and then evaluating the postfix expression.
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
                error = true;
                break;
            }
        }
        // Got a valid column letter. Check if it is a valid cell reference.
        else if ('A' <= *text && *text <= 'G')
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
                error = true;
                break;
            }
            // Not checking for circular dependency. Circular dependencies will be caught when updating children.
            cell_t *p = &table[parent_row][parent_col];
            // If we are dealing with a new input, then update to reflect new parent.
            if (new_input && !p->already_added)
            {
                // Add p to parents array of c1.
                c1->parents[c1->num_parents++] = (cell_pos_t){p, p->num_children};
                // Add c1 to children array of p.
                p->children[p->num_children++] = (cell_pos_t){c1, c1->num_parents - 1};
                // Mark this cell as already added so we don't add it again.
                p->already_added = true;
            }
            // Ensure this p is has data type NUMBER before continuing with the rest of the evaluation.
            if (p->data_type != NUMBER)
            {
                snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:REFNAN");
                c1->data_type = ERROR;
                error = true;
                // Don't break here since we want to still compute all the parent-child relationships.
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
            error = true;
            break;
        }
        // Overflow and underflow is not being checked.
        // Update the result based on the previous operator.
        switch (prev_op)
        {
        case '+':
            result += tmp_num;
            break;
        case '-':
            result -= tmp_num;
            break;
        // Should never reach here since we are checking for valid operators in the loop condition.
        default:
            assert(false);
            break;
        }
    }
    // Clear the already_added flag.
    for (size_t i = 0; i < c1->num_parents; i++)
        c1->parents[i].cell->already_added = false;

    // Check for invalid operator error.
    if (prev_op != '\0' && !error)
    {
        // Invalid character.
        snprintf(c1->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:SYNTAX");
        c1->data_type = ERROR;
        error = true;
    }
    // Return if there was an error reading the formula.
    if (error)
        return;

    // If the whole input was read correctly, then update the c1->text_display.
    c1->num = result;
    c1->data_type = NUMBER;
    cell_update_display_num(c1);
    return;
}

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
            // Initialize the data type as STRING.
            table[row][col].data_type = STRING;
            // Initalize the text_input string to "".
            table[row][col].text_input = strdup("");
            // Initialize the text_display string to empty.
            table[row][col].text_display[0] = '\0';
            // The children and parents array starts out as empty.
            table[row][col].num_children = table[row][col].num_parents = 0;
            // Set the flags to false.
            table[row][col].already_added = table[row][col].in_process = table[row][col].processed = table[row][col].circular_dependency = false;
            table[row][col].children_processed = 0;
        }
    }
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

    // Update the cell with it's new text_input. (This is repeated below when updating children, but this is necessary to calculate the parents of c1 if it's a formula.)
    update_cell(c1, true);

    // Perform updates on child cells and update cell displays in the interface.
    // A stack to store pointers to cells in the order that they should be updated.
    stack_t *update_order = stack_new();
    // A stack to store the cells that are currently being processed during depth-first search.
    stack_t *recursion = stack_new();
    // The following do-while loop will result in the cells with no children (farthest descendents of c1) being at the bottom of the update_order stack, and the cells with the most descendents (direct descendents of c1 and c1 itself) being at the top of the stack.
    // This also flags any cells that are causing a circular dependency.
    // Loop while the recursion stack is not empty.
    do
    {
        // Skip over children that have already been processed or are currently in_process.
        while (c1->children_processed < c1->num_children && (c1->children[c1->children_processed].cell->processed || c1->children[c1->children_processed].cell->in_process))
        {
            // If the child is already in process, we know that the child must be circularly referring to c1.
            if (c1->children[c1->children_processed].cell->in_process)
            {
                // 'c1' is the circular parent of this child.
                // Let the user know that this cell is causing a circular dependency so they can fix the error more directly.
                snprintf(c1->children[c1->children_processed].cell->text_display, CELL_DISPLAY_WIDTH + 1, "ERR:CIR:%c%d", c1->c + 'A', c1->r + 1);
                c1->children[c1->children_processed].cell->data_type = ERROR;
                c1->children[c1->children_processed].cell->circular_dependency = true;
            }
            c1->children_processed++;
        }
        // If all children have been processed, then we can add the current cell to the update_order stack and we can continue.
        if (c1->children_processed == c1->num_children)
        {
            stack_push(update_order, c1);
            c1->processed = true;
            c1->in_process = false;
            // reset the children_processed counter for the next time this function is called.
            c1->children_processed = 0;
            continue;
        }
        // Put the current cell on the stack since it's children have not all been processed yet.
        stack_push(recursion, c1);
        c1->in_process = true;
        // Put the next child on the stack.
        assert(c1->children[c1->children_processed].cell != NULL);
        stack_push(recursion, c1->children[c1->children_processed].cell);
        c1->children[c1->children_processed].cell->in_process = true;
        // If this child was causing a circular dependency, then it would have been skipped in the above while loop.
        c1->children[c1->children_processed].cell->circular_dependency = false;

    } while (stack_pop(recursion, &c1));
    // Free the recursion stack.
    stack_free(recursion);
    // loop while the update order stack is not empty.
    // Pop of each cell from the stack and update it.
    // This ordering is such that as each cell is popped from the stack, all of it's parents have been updated already.
    while (stack_pop(update_order, &c1))
    {
        assert(c1 != NULL);
        // reset the processed flag.
        c1->processed = false;
        // If there was no circular dependency, then update the cell by recalculating the formula.
        if (!c1->circular_dependency)
            update_cell(c1, false);
        // Update the display text in the interface.
        update_cell_display(c1->r, c1->c, c1->text_display);
    }
    // Free the update_order stack.
    stack_free(update_order);
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
