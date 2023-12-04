#include <string.h> // for strdup
#include <stdio.h>  // for snprintf and printf
#include <limits.h> // for CHAR_MAX
#include <ctype.h>  // for isdigit
#include <stdlib.h> // for rand and srand
#include <time.h>   // for time

#include "tests.h"      // implements this file
#include "model.h"      // for set_cell_value
#include "testrunner.h" // for assert_display_text and assert_edit_text
#define CELL_EDIT_WIDTH 1024

void run_tests()
{
    // Test setting a cell value as a number and check the display text.
    set_cell_value(ROW_2, COL_A, strdup("1.4")); // A2 = 1.4
    assert_display_text(ROW_2, COL_A, strdup("1.4"));
    printf("1\n");
    // Test setting a cell value as a number and check the display text.
    set_cell_value(ROW_2, COL_B, strdup("2.9")); // B2 = 2.9
    assert_display_text(ROW_2, COL_B, strdup("2.9"));
    printf("2\n");
    // Test the edit text representation of a valid formula. THIS TESTS FR2.b
    set_cell_value(ROW_2, COL_C, strdup("=A2+B2+0.4")); // C2 = A2 + B2 + 0.4 = 1.4 + 2.9 + 0.4 = 4.7
    assert_edit_text(ROW_2, COL_C, strdup("=A2+B2+0.4"));
    printf("3\n");
    // Test the display text representation of a valid formula.
    assert_display_text(ROW_2, COL_C, strdup("4.7"));
    printf("4\n");
    // Test that the value of a formula is updated when the cells it references are updated. THIS TESTS FR4
    set_cell_value(ROW_2, COL_B, strdup("3.1"));      // B2 = 3.1
    assert_display_text(ROW_2, COL_C, strdup("4.9")); // C2 = A2 + B2 + 0.4 = 1.4 + 3.1 + 0.4 = 4.9
    printf("5\n");
    // Test the use of subtraction. -- EXTRA FEATURE --
    set_cell_value(ROW_2, COL_C, strdup("=0.0-A2-B2-0.4")); // C2 = 0.0 - A2 - B2 - 0.4 = 0.0 - 1.4 - 3.1 - 0.4 = -4.9
    assert_edit_text(ROW_2, COL_C, strdup("=0.0-A2-B2-0.4"));
    assert_display_text(ROW_2, COL_C, strdup("-4.9"));
    printf("6\n");
    // Test the use of an invalid operator. (which is not implemented).
    set_cell_value(ROW_2, COL_C, strdup("=A2*B2")); // C2 = A2 * B2
    assert_display_text(ROW_2, COL_C, strdup("ERR:SYNTAX"));
    printf("7\n");
    // Test when the last character is invalid.
    set_cell_value(ROW_2, COL_C, strdup("=A2+B2+0.4+")); // C2 = A2 + B2 + 0.4 + -- INVALID FORMULA
    assert_edit_text(ROW_2, COL_C, strdup("=A2+B2+0.4+"));
    assert_display_text(ROW_2, COL_C, strdup("ERR:SYNTAX"));
    printf("8\n");
    // Test error when referencing a non number cell.
    set_cell_value(ROW_2, COL_A, strdup("hello")); // A2 = hello
    assert_display_text(ROW_2, COL_A, strdup("hello"));
    // Check that the error is displayed in cell C2.
    set_cell_value(ROW_2, COL_C, strdup("=A2+B2+0.4")); // C2 = A2 + B2 + 0.4 = hello + 3.1 + 0.4 = ERR:REF:NAN
    assert_display_text(ROW_2, COL_C, strdup("ERR:REFNAN"));
    printf("9\n");
    // Test error when referencing a cell that does not exist (out of range).
    set_cell_value(ROW_2, COL_A, strdup("=A30")); // A2 = A30
    assert_display_text(ROW_2, COL_A, strdup("ERR:REF"));
    assert_display_text(ROW_2, COL_C, strdup("ERR:REFNAN"));
    printf("10\n");
    // Clear the cells
    clear_cell(ROW_2, COL_A);
    clear_cell(ROW_2, COL_B);
    clear_cell(ROW_2, COL_C);
    assert_display_text(ROW_2, COL_A, strdup(""));
    assert_display_text(ROW_2, COL_B, strdup(""));
    assert_display_text(ROW_2, COL_C, strdup(""));
    printf("11\n");
    printf("FR1 Satisfied\n");

    // Test FR2.a: A value that starts with an equals sign (=) is always interpreted as a formula.
    // Test that any character after the equals sign is interpreted as part of a formula.
    for (int c = 0; c < CHAR_MAX; c++)
    {
        char formula[5] = {'=', c, '\0', '\0', '\0'};
        set_cell_value(ROW_2, COL_B, strdup(formula));
        // Filling in the .0 since each number is displayed with one digit after the decimal point.
        formula[2] = '.';
        formula[3] = '0';
        if (isdigit(c))
            assert_display_text(ROW_2, COL_B, strdup(formula + 1)); // Notice that the display text is the number in the formula.
        else if ('A' <= c && c <= 'G')
            assert_display_text(ROW_2, COL_B, strdup("ERR:REF")); // invalid reference error
        else
            assert_display_text(ROW_2, COL_B, strdup("ERR:SYNTAX")); // syntax error
    }
    // If any of the assertions fail, then the the cell is must not being interpreted as a formula.
    printf("FR2.a Satisfied\n");
    // FR2.b: A valid formula consists of one or more numbers of cell coordinates (e.g. B7) separated by addition signs (+). With the added feature of subtraction we can also have subtraction signs (-).
    // This was tested earlier on line 20 of this file.
    printf("FR2.b Satisfied\n");

    // FR2.c: A value which consists of one or more decimal digits (0 through 9) with at most one decimal point is interpreted as a number.
    // Test 100 random numbers.
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 100; i++)
    {
        char input[CELL_EDIT_WIDTH + 1];
        float number = ((float)rand() / (float)(RAND_MAX)) * 1e5f;
        snprintf(input, CELL_EDIT_WIDTH + 1, "%.3f", number); // simulate user putting 3 digits after the decimal point.
        sscanf(input, "%f", &number);                         // convert the string to a float.
        set_cell_value(ROW_2, COL_B, strdup(input));
        snprintf(input, CELL_EDIT_WIDTH + 1, "%.1f", number); // number with 1 only digit after the decimal point should be what is in the display text.
        assert_display_text(ROW_2, COL_B, strdup(input));
        printf("number worked: %.3f\n", number);
    }
    printf("FR2.c Satisfied\n");

    // FR2.d: A value which does not start with an equals sign and is not a number is interpreted as text.
    // Test 100 random strings.
    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}\\|;:'\",<.>/?`~";
    // Don't forget that it's possible a string gets read as a number if it starts with a number and contains only numbers, or if it is in scientific notation.
    for (int i = 0; i < 100; i++)
    {
        char input[CELL_EDIT_WIDTH + 1];
        for (int j = 0; j < CELL_EDIT_WIDTH; j++)
        {
            input[j] = characters[(char)(rand() % (sizeof(characters)))];
        }
        if (input[0] == '=')
            input[0] = ' '; // replace the equals sign with a space so that it is not interpreted as a formula.
        input[CELL_EDIT_WIDTH] = '\0';
        set_cell_value(ROW_2, COL_B, strdup(input));
        input[CELL_DISPLAY_WIDTH] = '\0'; // truncate the string to the display width for comparison.
        assert_display_text(ROW_2, COL_B, strdup(input));
        printf("%d, string worked: %s\n", i, input);
    }
    printf("FR2.d Satisfied\n");
    // Test FR3: When a user navigates to a cell, a textual representation of its value is shown in an editable field (i.e., the content field at the top of Excel’s interface), and the user can edit the value of the cell based on that representation. When the cell contains a formula, this should be a representation of the formula itself, not the computed value (which is shown in the cell itself).
    // This was tested earlier on in lines 25, 35, and 44 of this file.
    printf("FR3 Satisfied\n");

    // Test FR4: When the value of a cell changes, the displayed contents of all formula cells which (directly or indirectly) depend on it are updated.
    // This was tested earlier on in line 31 of this file. I will perform an additional tests here.
    // populate cell A1 with a constant value.
    set_cell_value(ROW_1, COL_A, strdup("1"));
    // populate each other cell with a formula that references all the cells before it. This is the worst case scenario for updating cells. This is the most dependencies a cell and all of it's descendants can have without circular dependencies.
    char formula[CELL_EDIT_WIDTH] = "=0";
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS, prev_r = (i - 1) / NUM_COLS;
        COL c = i % NUM_COLS, prev_c = (i - 1) % NUM_COLS;
        snprintf(formula, CELL_EDIT_WIDTH, "%s+%c%d", formula, prev_c + 'A', prev_r + 1); // prev_r + 1 since ROW_1==0.
        set_cell_value(r, c, strdup(formula));
    }
    // Check that all the cells have the correct values.
    assert_display_text(ROW_1, COL_A, strdup("1.0"));
    float num = 1;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }
    // Perform an update on cell A1 and check that all the cells have the correct values.
    set_cell_value(ROW_1, COL_A, strdup("2.5"));
    assert_display_text(ROW_1, COL_A, strdup("2.5"));
    num = 2.5;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }

    // Now to test circular dependencies.

    // Test simplest circular dependency, self reference.
    set_cell_value(ROW_1, COL_A, strdup("=A1")); // A1 = A1
    // Check that the errors are displayed correctly in all the cells.
    assert_display_text(ROW_1, COL_A, strdup("ERR:CIR:A1"));
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        assert_display_text(r, c, strdup("ERR:REFNAN"));
    }
    // Fix the error.
    set_cell_value(ROW_1, COL_A, strdup("1.0"));
    assert_display_text(ROW_1, COL_A, strdup("1.0"));
    num = 1;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }
    // Create the longest chain of circular dependency possible.
    set_cell_value(ROW_1, COL_A, strdup("=G10")); // A1 = G10
    // Check that the errors are displayed correctly in all the cells.
    assert_display_text(ROW_1, COL_A, strdup("ERR:CIR:G10"));
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        assert_display_text(r, c, strdup("ERR:REFNAN"));
    }
    // Fix the error.
    set_cell_value(ROW_1, COL_A, strdup("123.0")); // A1 = 123
    // Ensure that all the cells have the correct values after fixing the error.
    assert_display_text(ROW_1, COL_A, strdup("123.0"));
    num = 123;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }
    // Create a circular dependency that is not the longest possible.
    set_cell_value(ROW_1, COL_A, strdup("=D6")); // A1 = D6
    // Check that the errors are displayed correctly in all the cells.
    assert_display_text(ROW_1, COL_A, strdup("ERR:CIR:D6"));
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        assert_display_text(r, c, strdup("ERR:REFNAN"));
    }
    // Fix the error.
    set_cell_value(ROW_1, COL_A, strdup("3.3")); // A1 = 3.3
    // Ensure that all the cells have the correct values after fixing the error.
    assert_display_text(ROW_1, COL_A, strdup("3.3"));
    num = 3.3;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }
    // Create a circular dependency somwhere in the middle of the dependency chain.
    set_cell_value(ROW_4, COL_C, strdup("=F8")); // C4 = F8
    // This should result in all the cells from A1 to C4 to have the correct values. Cell C4 should be ERR:CIR:F8 and all the cells from C4 to G10 should be ERR:REFNAN.
    assert_display_text(ROW_1, COL_A, strdup("3.3"));
    num = 3.3;
    for (int i = 1; i < NUM_ROWS * NUM_COLS; i++)
    {
        ROW r = i / NUM_COLS;
        COL c = i % NUM_COLS;
        char expected[CELL_DISPLAY_WIDTH + 1];
        if (i < ROW_4 * NUM_COLS + COL_C)
            snprintf(expected, CELL_DISPLAY_WIDTH + 1, "%.1f", num);
        else if (i == ROW_4 * NUM_COLS + COL_C)
            snprintf(expected, CELL_DISPLAY_WIDTH + 1, "ERR:CIR:F8");
        else
            snprintf(expected, CELL_DISPLAY_WIDTH + 1, "ERR:REFNAN");
        assert_display_text(r, c, strdup(expected));
        num *= 2;
    }
    // Perhaps additional tests could be done with multiple circular dependencies, but for now I will leave it at this.
    printf("FR4 Satisfied\n");
}
