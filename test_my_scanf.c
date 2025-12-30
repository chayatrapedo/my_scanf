#include "my_scanf.h"
#include <stdio.h>
#include <unistd.h>

// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Colors
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"


// Compare scanf and my_scanf with identical file input for %d
int test_scanf_int(const char *test_name, const char *input_file) {
    tests_run++;

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val, my_scanf_val;

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

    int returns_match = (scanf_ret == my_scanf_ret);
    int values_match = (scanf_val == my_scanf_val);

    // Final verdict
    char* pass_fail = ((returns_match && values_match)) ? "PASS" : "FAIL";
    printf("Results: %s\n", pass_fail);
    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);
    printf("\tmy_scanf() returned: %d, value: %d\n", my_scanf_ret, my_scanf_val);

    if (returns_match && values_match) {
        tests_passed++;
        printf("***\n");
        return 1;
    } else {
        if (!returns_match) {
            printf("%s  Return values don't match!\n%s***\n", COLOR_RED, COLOR_RESET);
        }
        if (!values_match) {
            printf("%s  Parsed values don't match!\n%s***\n", COLOR_RED, COLOR_RESET);
        }
        tests_failed++;
        return 0;
    }
}

// Test multiple integers with format "%d %d"
int test_scanf_two_ints(const char *test_name, const char *input_file) {
    tests_run++;

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val1, scanf_val2;
    int my_scanf_val1, my_scanf_val2;

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

    int returns_match = (scanf_ret == my_scanf_ret);
    int values_match = (scanf_val1 == my_scanf_val1) && (scanf_val2 == my_scanf_val2);

    // Final verdict
    char* pass_fail = ((returns_match && values_match)) ? "PASS" : "FAIL";
    printf("Results: %s\n", pass_fail);
    printf("\tscanf()    returned: %d, values: %d %d\n", scanf_ret, scanf_val1, scanf_val2);
    printf("\tmy_scanf() returned: %d, values: %d %d\n", my_scanf_ret, my_scanf_val1, my_scanf_val2);

    if (returns_match && values_match) {
        tests_passed++;
        printf("***\n");
        return 1;
    } else {
        if (!returns_match) {
            printf("%s  Return values don't match!%s\n***\n", COLOR_RED, COLOR_RESET);
        }
        if (!values_match) {
            printf("%s  Parsed values don't match!\n%s***\n", COLOR_RED, COLOR_RESET);
        }
        tests_failed++;
        return 0;
    }
}


// Test three integers with format "%d %d %d"
int test_scanf_three_ints(const char *test_name, const char *input_file) {
    tests_run++;

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val1, scanf_val2, scanf_val3;
    int my_scanf_val1, my_scanf_val2, my_scanf_val3;

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

    int returns_match = (scanf_ret == my_scanf_ret);
    int values_match = (scanf_val1 == my_scanf_val1) &&
                       (scanf_val2 == my_scanf_val2) &&
                       (scanf_val3 == my_scanf_val3);

    // Final verdict
    char* pass_fail = ((returns_match && values_match)) ? "PASS" : "FAIL";
    printf("Results: %s\n", pass_fail);
    printf("\tscanf()    returned: %d, values: %d %d %d\n", scanf_ret, scanf_val1, scanf_val2, scanf_val3);
    printf("\tmy_scanf() returned: %d, values: %d %d %d\n", my_scanf_ret, my_scanf_val1, my_scanf_val2, my_scanf_val3);

    if (returns_match && values_match) {
        tests_passed++;
        printf("***\n");
        return 1;
    } else {
        if (!returns_match) {
            printf("%s  Return values don't match!%s\n***\n", COLOR_RED, COLOR_RESET);
        }
        if (!values_match) {
            printf("%s  Parsed values don't match!%s\n***\n", COLOR_RED, COLOR_RESET);
        }
        tests_failed++;
        return 0;
    }
}

int main() {
    printf("  MY_SCANF TEST SUITE\n");

    // Test %d (integers)
    printf("\n=== %%d Tests ==========================\n");

    test_scanf_int("Positive integer", "test_inputs/test_pos.txt");
    test_scanf_int("Negative integer", "test_inputs/test_neg.txt");
    test_scanf_int("Zero", "test_inputs/test_zero.txt");
    test_scanf_int("Large number", "test_inputs/test_large.txt");

    // Boundary values
    test_scanf_int("INT_MAX", "test_inputs/test_int_max.txt");
    test_scanf_int("INT_MIN", "test_inputs/test_int_min.txt");

    // Whitespace handling
    test_scanf_int("Leading spaces", "test_inputs/test_leading_space.txt");
    test_scanf_int("Leading tab", "test_inputs/test_leading_tab.txt");
    test_scanf_int("Multiple spaces", "test_inputs/test_multiple_spaces.txt");

    // Sign variations
    test_scanf_int("Plus sign", "test_inputs/test_plus_sign.txt");

    // Invalid input (should fail gracefully)
    test_scanf_int("Letters only", "test_inputs/test_letters.txt");
    test_scanf_int("Partial number", "test_inputs/test_partial_number.txt");
    test_scanf_int("Letters first", "test_inputs/test_letters_first.txt");
    test_scanf_int("Empty input", "test_inputs/test_empty.txt");
    test_scanf_int("Decimal point", "test_inputs/test_decimal.txt");

    // Trailing characters
    test_scanf_int("Trailing letter", "test_inputs/test_trailing_letter.txt");
    test_scanf_int("Trailing text", "test_inputs/test_trailing_text.txt");


    // Multiple integers
    test_scanf_two_ints("Two integers", "test_inputs/test_two_ints.txt");
    test_scanf_three_ints("Three integers", "test_inputs/test_three_ints.txt");

    // Print summary
    printf("\n========================================\n");
    printf("  TEST SUMMARY\n");
    printf("Tests run:    %d\n", tests_run);
    printf("Passed:       %d\n", tests_passed);
    printf("Failed:       %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}