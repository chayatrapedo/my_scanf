#include "my_scanf.h"
#include <stdio.h>
#include <unistd.h>

// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Colors
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RESET "\033[0m"

typedef enum {
    EXPECT_SUCCESS,   // Both should successfully read
    EXPECT_FAILURE    // Both should fail to read
} ExpectedBehavior;

void reset_stdin() {
    fflush(stdin);   // Try to clear the buffer
    rewind(stdin);   // Go back to start of file
    clearerr(stdin); // Clear any EOF/error flags
}


// Helper: Check if behavior matches expectations
int check_behavior_match(int scanf_ret, int my_scanf_ret, int scanf_val, int my_scanf_val, ExpectedBehavior expected) {

    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret == 0 && my_scanf_ret == 0);
    int values_match = (scanf_val == my_scanf_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n",
           expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");

    // Check if behaviors match
    int behaviors_match = returns_match && (both_failed || values_match);
    printf("Behavior match: %s\n", behaviors_match ? "YES" : "NO");

    // Determine if test passed
    if (expected == EXPECT_SUCCESS) {
        if (both_succeeded && values_match) {
            printf("Result: %sPASS%s (both succeeded with matching values)\n",
                   COLOR_GREEN, COLOR_RESET);
            return 1;
        } else if (both_failed) {
            printf("Result: %sFAIL%s (both failed but input should be valid)\n",
                   COLOR_RED, COLOR_RESET);
            return 0;
        } else {
            printf("Result: %sFAIL%s (functions behaved differently)\n",
                   COLOR_RED, COLOR_RESET);
            return 0;
        }
    } else {
        if (both_failed) {
            printf("Result: %sPASS%s (both failed as expected)\n",
                   COLOR_GREEN, COLOR_RESET);
            return 1;
        } else if (both_succeeded && values_match) {
            printf("Result: %sFAIL%s (both succeeded but should have failed)\n",
                   COLOR_RED, COLOR_RESET);
            return 0;
        } else {
            printf("Result: %sFAIL%s (functions behaved differently)\n",
                   COLOR_RED, COLOR_RESET);
            return 0;
        }
    }
}

// Compare scanf and my_scanf with identical file input for %d
int test_scanf_int(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    reset_stdin();

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val = -999;      // Initialize to pre-set values; make sure no leftover
    int my_scanf_val = -999;

    // Test scanf() - open file and redirect stdin
    FILE *fp1 = fopen(input_file, "r");
    if (!fp1) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    //  Save original stdin and redirect stdin to file
    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp1), STDIN_FILENO);

    // Test against a real scanf
    int scanf_ret = scanf("%d", &scanf_val);

    // Restore stdin
    dup2(saved_stdin, STDIN_FILENO);
    fclose(fp1);

    // Test my_scanf() - reopen test file and read from beginning
    FILE *fp2 = fopen(input_file, "r");
    if (!fp2) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        close(saved_stdin);
        return 0;
    }

    // Redirect stdin to file again
    dup2(fileno(fp2), STDIN_FILENO);
    int my_scanf_ret = my_scanf("%d", &my_scanf_val);

    // Restore stdin
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp2);

    // Print results
    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);
    printf("\tmy_scanf() returned: %d, value: %d\n", my_scanf_ret, my_scanf_val);

    // Check behavior
    int passed = check_behavior_match(scanf_ret, my_scanf_ret, scanf_val, my_scanf_val, expected);
    if (passed) {
        tests_passed++;
    } else {
        tests_failed++;
    }
    printf("***\n");
    return passed;
}

// Test multiple integers with format "%d %d"
int test_scanf_two_ints(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    reset_stdin();

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val1 = -999, scanf_val2 = -999;           // Initialize to preset values
    int my_scanf_val1 = -999, my_scanf_val2 = -999;


    // Test scanf() - open file and redirect stdin
    FILE *fp1 = fopen(input_file, "r");
    if (!fp1) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp1), STDIN_FILENO);

    int scanf_ret = scanf("%d %d", &scanf_val1, &scanf_val2);

    dup2(saved_stdin, STDIN_FILENO);
    fclose(fp1);

    // Test my_scanf() - reopen file
    FILE *fp2 = fopen(input_file, "r");
    if (!fp2) {
        printf("%sFAIL%s: Could not reopen input file\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
        close(saved_stdin);
        return 0;
    }

    dup2(fileno(fp2), STDIN_FILENO);

    int my_scanf_ret = my_scanf("%d %d", &my_scanf_val1, &my_scanf_val2);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp2);

    // For multi-value tests, check all values match
    int values_match = (scanf_val1 == my_scanf_val1) && (scanf_val2 == my_scanf_val2);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && returns_match && values_match && scanf_ret == 2) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else if (expected == EXPECT_FAILURE && scanf_ret == 0 && my_scanf_ret == 0) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
    }

    printf("***\n");
    return passed;
}


// Test three integers with format "%d %d %d"
int test_scanf_three_ints(const char *test_name, const char *input_file, ExpectedBehavior expected){
    tests_run++;
    reset_stdin();

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val1 = -999, scanf_val2 = -999, scanf_val3 = -999;          // Initialize to preset values
    int my_scanf_val1 = -999, my_scanf_val2 = -999, my_scanf_val3 = -999;

    // Test scanf()
    FILE *fp1 = fopen(input_file, "r");
    if (!fp1) {
        printf("%sFAIL%s: Could not open input file\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp1), STDIN_FILENO);

    int scanf_ret = scanf("%d %d %d", &scanf_val1, &scanf_val2, &scanf_val3);

    dup2(saved_stdin, STDIN_FILENO);
    fclose(fp1);

    // Test my_scanf()
    FILE *fp2 = fopen(input_file, "r");
    if (!fp2) {
        printf("%sFAIL%s: Could not reopen input file\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
        close(saved_stdin);
        return 0;
    }

    dup2(fileno(fp2), STDIN_FILENO);

    int my_scanf_ret = my_scanf("%d %d %d", &my_scanf_val1, &my_scanf_val2, &my_scanf_val3);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp2);

    // Print results
    printf("scanf()    returned: %d, values: %d %d %d\n",
           scanf_ret, scanf_val1, scanf_val2, scanf_val3);
    printf("my_scanf() returned: %d, values: %d %d %d\n",
           my_scanf_ret, my_scanf_val1, my_scanf_val2, my_scanf_val3);

    int values_match = (scanf_val1 == my_scanf_val1) &&
                       (scanf_val2 == my_scanf_val2) &&
                       (scanf_val3 == my_scanf_val3);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && returns_match && values_match && scanf_ret == 3) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else if (expected == EXPECT_FAILURE && scanf_ret == 0 && my_scanf_ret == 0) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
    }

    printf("***\n");
    return passed;
}

int main() {
    printf("  MY_SCANF TEST SUITE\n");
    printf("\n=== %%d Tests ==========================\n");

    // Valid inputs - should succeed
    test_scanf_int("Positive integer", "test_inputs/test_pos.txt", EXPECT_SUCCESS);
    test_scanf_int("Negative integer", "test_inputs/test_neg.txt", EXPECT_SUCCESS);
    test_scanf_int("Zero", "test_inputs/test_zero.txt", EXPECT_SUCCESS);
    test_scanf_int("Large number", "test_inputs/test_large.txt", EXPECT_SUCCESS);
    test_scanf_int("INT_MAX", "test_inputs/test_int_max.txt", EXPECT_SUCCESS);
    test_scanf_int("INT_MIN", "test_inputs/test_int_min.txt", EXPECT_SUCCESS);
    test_scanf_int("Leading spaces", "test_inputs/test_leading_space.txt", EXPECT_SUCCESS);
    test_scanf_int("Leading tab", "test_inputs/test_leading_tab.txt", EXPECT_SUCCESS);
    test_scanf_int("Multiple spaces", "test_inputs/test_multiple_spaces.txt", EXPECT_SUCCESS);
    test_scanf_int("Plus sign", "test_inputs/test_plus_sign.txt", EXPECT_SUCCESS);

    // Edge cases - partial reads (should succeed with partial value)
    test_scanf_int("Partial number", "test_inputs/test_partial_number.txt", EXPECT_SUCCESS);
    test_scanf_int("Decimal point", "test_inputs/test_decimal.txt", EXPECT_SUCCESS);
    test_scanf_int("Trailing letter", "test_inputs/test_trailing_letter.txt", EXPECT_SUCCESS);
    test_scanf_int("Trailing text", "test_inputs/test_trailing_text.txt", EXPECT_SUCCESS);

    // Invalid inputs - should fail
    test_scanf_int("Letters only", "test_inputs/test_letters.txt", EXPECT_FAILURE);
    test_scanf_int("Letters first", "test_inputs/test_letters_first.txt", EXPECT_FAILURE);
    test_scanf_int("Empty input", "test_inputs/test_empty.txt", EXPECT_FAILURE);

    // Multiple integers
    test_scanf_two_ints("Two integers", "test_inputs/test_two_ints.txt", EXPECT_SUCCESS);
    test_scanf_three_ints("Three integers", "test_inputs/test_three_ints.txt", EXPECT_SUCCESS);

    // Print summary
    printf("\n========================================\n");
    printf("TEST SUMMARY\n");
    printf("  Tests run:    %d\n", tests_run);
    printf("  %sPassed:       %d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("  %sFailed:       %d%s\n", COLOR_RED, tests_failed, COLOR_RESET);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}