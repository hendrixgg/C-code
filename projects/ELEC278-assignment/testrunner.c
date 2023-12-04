#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"  // implements this file
#include "testrunner.h" // implements this file
#include "defs.h"
#include "model.h"
#include "tests.h"

static char display[NUM_ROWS][NUM_COLS][CELL_DISPLAY_WIDTH + 1];

int main()
{
    memset(display, 0, sizeof(display));
    // Probaly going to need to call model_init() before running tests.
    model_init();
    run_tests();
    return 0;
}

void update_cell_display(ROW row, COL col, const char *text)
{
    snprintf(display[row][col], CELL_DISPLAY_WIDTH + 1, "%s", text);
}

/**
 * Asserts that the text displayed at the given row and column is equal to the given text.
 *
 * If the assertion is to fail, the expected and actual values are printed to the console.
 * @param row the row of the cell to check
 * @param col the column of the cell to check
 * @param text the expected text. This string is freed after the assertion is made.
 */
void assert_display_text(ROW row, COL col, char *text)
{
    if (strcmp(text, display[row][col]) != 0)
    {
        printf("Expected: %s\n", text);
        printf("Actual: %s\n", display[row][col]);
    }
    assert(strcmp(text, display[row][col]) == 0);
    free(text);
}

/**
 * Asserts that the edit text at the given row and column is equal to the given text.
 *
 * If the assertion is to fail, the expected and actual values are printed to the console.
 * @param row the row of the cell to check
 * @param col the column of the cell to check
 * @param text the expected text. This string is freed after the assertion is made.
 */
void assert_edit_text(ROW row, COL col, char *text)
{
    char *value = get_textual_value(row, col);
    if (value != NULL && strcmp(text, value) != 0)
    {
        printf("Expected: %s\n", text);
        printf("Actual: %s\n", value);
    }
    assert(value != NULL && strcmp(text, value) == 0);
    free(value);
    free(text);
}
