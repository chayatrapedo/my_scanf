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
    EXPECT_SUCCESS,
    EXPECT_FAILURE
} ExpectedBehavior;

// Helper: Check if behavior matches expectations
int check_behavior_match(int scanf_ret, int my_scanf_ret, int scanf_val, int my_scanf_val, ExpectedBehavior expected) {
    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret == 0 && my_scanf_ret == 0) || (scanf_ret == -1 && my_scanf_ret == -1);
    int values_match = (scanf_val == my_scanf_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n",
           expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

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

int test_scanf_int(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);
    clearerr(stdin);  // Clear EOF/error flags from stdin


    int scanf_val = -999;
    int scanf_ret = scanf("%d", &scanf_val);

    fflush(stdin);
    fclose(fp);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);

    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);

    // Test my_scanf()
    fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);

    int my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%d", &my_scanf_val);

    fflush(stdin);
    fclose(fp);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);

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

int test_scanf_two_ints(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);
    clearerr(stdin);  // Clear EOF/error flags from stdin


    int scanf_val1 = -999, scanf_val2 = -999;
    int scanf_ret = scanf("%d %d", &scanf_val1, &scanf_val2);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    // Test my_scanf()
    fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);

    int my_scanf_val1 = -999, my_scanf_val2 = -999;
    int my_scanf_ret = my_scanf("%d %d", &my_scanf_val1, &my_scanf_val2);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    // Check results
    int values_match = (scanf_val1 == my_scanf_val1) && (scanf_val2 == my_scanf_val2);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("scanf()    returned: %d, values: %d %d\n", scanf_ret, scanf_val1, scanf_val2);
    printf("my_scanf() returned: %d, values: %d %d\n", my_scanf_ret, my_scanf_val1, my_scanf_val2);
    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && returns_match && values_match && scanf_ret == 2 && my_scanf_ret == 2) {
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

int test_scanf_three_ints(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);
    clearerr(stdin);  // Clear EOF/error flags from stdin


    int scanf_val1 = -999, scanf_val2 = -999, scanf_val3 = -999;
    int scanf_ret = scanf("%d %d %d", &scanf_val1, &scanf_val2, &scanf_val3);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    // Test my_scanf()
    fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    saved_stdin = dup(STDIN_FILENO);
    dup2(fileno(fp), STDIN_FILENO);
    fflush(stdout);

    int my_scanf_val1 = -999, my_scanf_val2 = -999, my_scanf_val3 = -999;
    int my_scanf_ret = my_scanf("%d %d %d", &my_scanf_val1, &my_scanf_val2, &my_scanf_val3);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    // Check results
    int values_match = (scanf_val1 == my_scanf_val1) &&
                       (scanf_val2 == my_scanf_val2) &&
                       (scanf_val3 == my_scanf_val3);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("scanf()    returned: %d, values: %d %d %d\n",
           scanf_ret, scanf_val1, scanf_val2, scanf_val3);
    printf("my_scanf() returned: %d, values: %d %d %d\n",
           my_scanf_ret, my_scanf_val1, my_scanf_val2, my_scanf_val3);
    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && returns_match && values_match && scanf_ret == 3 && my_scanf_ret == 3) {
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

int test_scanf_char(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int fd = fileno(fp);
    int saved_stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);

    char scanf_val = '\0';
    int scanf_ret = scanf("%c", &scanf_val);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    printf("\tscanf()    returned: %d, value: %c\n", scanf_ret, scanf_val);

    // Test my_scanf()
    fp = fopen(input_file, "r");
    if (!fp) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    fd = fileno(fp);
    saved_stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);

    char my_scanf_val = '\0';
    int my_scanf_ret = my_scanf("%c", &my_scanf_val);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    fclose(fp);

    printf("\tmy_scanf() returned: %d, value: %c\n", my_scanf_ret, my_scanf_val);

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

    // Edge cases
    test_scanf_int("Partial number", "test_inputs/test_partial_number.txt", EXPECT_SUCCESS);  // 123abc -> reads 123
    test_scanf_int("Decimal point", "test_inputs/test_decimal.txt", EXPECT_SUCCESS);           // 12.34 -> reads 12
    test_scanf_int("Trailing letter", "test_inputs/test_trailing_letter.txt", EXPECT_SUCCESS); // 42x -> reads 42
    test_scanf_int("Trailing text", "test_inputs/test_trailing_text.txt", EXPECT_SUCCESS);     // 42 hello -> reads 42

    // Invalid inputs - should fail
    test_scanf_int("Letters only", "test_inputs/test_letters.txt", EXPECT_FAILURE);
    test_scanf_int("Letters first", "test_inputs/test_letters_first.txt", EXPECT_FAILURE);
    test_scanf_int("Empty input", "test_inputs/test_empty.txt", EXPECT_FAILURE);

    // Multiple integers
    test_scanf_two_ints("Two integers", "test_inputs/test_two_ints.txt", EXPECT_SUCCESS);
    test_scanf_three_ints("Three integers", "test_inputs/test_three_ints.txt", EXPECT_SUCCESS);

    printf("\n=== %%c Tests ==========================\n");
    test_scanf_char("One character", "test_inputs/test_char_a.txt", EXPECT_SUCCESS);
    test_scanf_char("Space", "test_inputs/test_char_space.txt", EXPECT_SUCCESS);
    test_scanf_char("Digit", "test_inputs/test_char_digit.txt", EXPECT_SUCCESS);



    // Print summary
    printf("\n========================================\n");
    printf("TEST SUMMARY\n");
    printf("  Tests run:    %d\n", tests_run);
    printf("  %sPassed:       %d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("  %sFailed:       %d%s\n", COLOR_RED, tests_failed, COLOR_RESET);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
/*
int main() {
    printf("Test 1: Empty input\n");
    FILE *fp = fopen("test_inputs/test_empty.txt", "r");
    int val = -999;
    int ret = scanf("%d", &val);
    printf("scanf returned: %d, value: %d\n", ret, val);
    fclose(fp);

    printf("\nTest 2: Two integers from file\n");
    fp = fopen("test_inputs/test_two_ints.txt", "r");
    int val1 = -999, val2 = -999;
    ret = scanf("%d", &val1);
    printf("First scanf returned: %d, value: %d\n", ret, val1);
    ret = scanf("%d", &val2);
    printf("Second scanf returned: %d, value: %d\n", ret, val2);
    fclose(fp);

    return 0;
}
*/