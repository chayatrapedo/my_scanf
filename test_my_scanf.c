#include "my_scanf.h"
#include <stdio.h>
#include <unistd.h>

// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/**
 * Compare scanf and my_scanf with identical file input for %d
 */
int test_scanf_int(const char *test_name, const char *input_file) {
    tests_run++;

    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    int scanf_val, my_scanf_val;

    // Test scanf() - open file and redirect stdin
    FILE *fp1 = fopen(input_file, "r");
    if (!fp1) {
        printf("FAIL: Could not open input file\n");
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
        printf("FAIL: Could not reopen input file\n");
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
        printf("FAIL\n");
        if (!returns_match) {
            printf("  Return values don't match!\n***\n");
        }
        if (!values_match) {
            printf("  Parsed values don't match!\n***\n");
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

    // Print summary
    printf("\n========================================\n");
    printf("  TEST SUMMARY\n");
    printf("Tests run:    %d\n", tests_run);
    printf("Passed:       %d\n", tests_passed);
    printf("Failed:       %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}