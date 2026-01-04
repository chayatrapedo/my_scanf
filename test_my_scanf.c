#include "my_scanf.h"
#include <stdio.h>
#include <string.h>

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

// Tests
int test_scanf_int(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val = -999;
    int scanf_ret = scanf("%d", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%d", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

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
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val1 = -999, scanf_val2 = -999;
    int scanf_ret = scanf("%d %d", &scanf_val1, &scanf_val2);

    freopen("/dev/tty", "r", stdin);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val1 = -999, my_scanf_val2 = -999;
    int my_scanf_ret = my_scanf("%d %d", &my_scanf_val1, &my_scanf_val2);

    freopen("/dev/tty", "r", stdin);

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
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val1 = -999, scanf_val2 = -999, scanf_val3 = -999;
    int scanf_ret = scanf("%d %d %d", &scanf_val1, &scanf_val2, &scanf_val3);

    freopen("/dev/tty", "r", stdin);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val1 = -999, my_scanf_val2 = -999, my_scanf_val3 = -999;
    int my_scanf_ret = my_scanf("%d %d %d", &my_scanf_val1, &my_scanf_val2, &my_scanf_val3);

    freopen("/dev/tty", "r", stdin);

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

int test_scanf_suppress_int(const char *test_name, const char *input_file, const char *format_str) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val1 = -999, scanf_val2 = -999;
    int scanf_ret = scanf(format_str, &scanf_val1, &scanf_val2);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %d\n", scanf_ret, scanf_val1, scanf_val2);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val1 = -999, my_scanf_val2 = -999;
    int my_scanf_ret = my_scanf(format_str, &my_scanf_val1, &my_scanf_val2);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %d\n", my_scanf_ret, my_scanf_val1, my_scanf_val2);

    // Check results
    int values_match = (scanf_val1 == my_scanf_val1) && (scanf_val2 == my_scanf_val2);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (returns_match && values_match) {
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

int test_scanf_long(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long scanf_val = -999;
    int scanf_ret = scanf("%ld", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %ld\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%ld", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %ld\n", my_scanf_ret, my_scanf_val);

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

int test_scanf_long_long(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long long scanf_val = -999;
    int scanf_ret = scanf("%lld", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %lld\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long long my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%lld", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %lld\n", my_scanf_ret, my_scanf_val);

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

int test_scanf_short(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    short scanf_val = -999;
    int scanf_ret = scanf("%hd", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %hd\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    short my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%hd", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %hd\n", my_scanf_ret, my_scanf_val);

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

int test_scanf_char(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val = '\0';
    int scanf_ret = scanf("%c", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %c\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val = '\0';
    int my_scanf_ret = my_scanf("%c", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

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

int test_scanf_suppress_char(const char *test_name, const char *input_file, const char *format_str) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val = '\0';
    int scanf_ret = scanf(format_str, &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '%c'\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val = '\0';
    int my_scanf_ret = my_scanf(format_str, &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%c'\n", my_scanf_ret, my_scanf_val);

    // Check results
    int values_match = (scanf_val == my_scanf_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (returns_match && values_match) {
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

int test_scanf_int_char(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_int_val = -999;
    char scanf_char_val = '\0';
    int scanf_ret = scanf("%d %c", &scanf_int_val, &scanf_char_val);

    freopen("/dev/tty", "r", stdin);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_int_val = -999;
    char my_scanf_char_val = '\0';
    int my_scanf_ret = my_scanf("%d %c", &my_scanf_int_val, &my_scanf_char_val);

    freopen("/dev/tty", "r", stdin);

    // Check results
    int values_match = (scanf_int_val == my_scanf_int_val) && (scanf_char_val == my_scanf_char_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("scanf()    returned: %d, values: %d '%c'\n", scanf_ret, scanf_int_val, scanf_char_val);
    printf("my_scanf() returned: %d, values: %d '%c'\n", my_scanf_ret, my_scanf_int_val, my_scanf_char_val);
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

int test_scanf_char_multi(const char *test_name, const char *input_file, const char *format_str, int num_chars, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val[256] = {0};
    int scanf_ret = scanf(format_str, scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '", scanf_ret);
    for (int i = 0; i < num_chars && scanf_ret > 0; i++) {
        printf("%c", scanf_val[i]);
    }
    printf("'\n");

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val[256] = {0};
    int my_scanf_ret = my_scanf(format_str, my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '", my_scanf_ret);
    for (int i = 0; i < num_chars && my_scanf_ret > 0; i++) {
        printf("%c", my_scanf_val[i]);
    }
    printf("'\n");

    // Check results
    int values_match = 1;
    for (int i = 0; i < num_chars; i++) {
        if (scanf_val[i] != my_scanf_val[i]) {
            values_match = 0;
            break;
        }
    }
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS) {
        if (returns_match && values_match && scanf_ret == 1) {
            printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    } else {
        if (scanf_ret == 0 && my_scanf_ret == 0) {
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    }

    printf("***\n");
    return passed;
}

int test_scanf_string(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val[256] = {0};
    int scanf_ret = scanf("%s", scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '%s'\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val[256] = {0};
    int my_scanf_ret = my_scanf("%s", my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%s'\n", my_scanf_ret, my_scanf_val);

    // Check behavior
    int values_match = (strcmp(scanf_val, my_scanf_val) == 0);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && returns_match && values_match && scanf_ret == 1) {
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

int test_scanf_string_with_format(const char *test_name, const char *input_file, const char *format_str) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val[256] = {0};
    int scanf_ret = scanf(format_str, scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '%s'\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val[256] = {0};
    int my_scanf_ret = my_scanf(format_str, my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%s'\n", my_scanf_ret, my_scanf_val);

    // Check results
    int values_match = (strcmp(scanf_val, my_scanf_val) == 0);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (returns_match && values_match && scanf_ret == 1) {
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

int test_scanf_string_suppress(const char *test_name, const char *input_file, const char *format_str) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char scanf_val[256] = {0};
    int scanf_ret = scanf(format_str, scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '%s'\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    char my_scanf_val[256] = {0};
    int my_scanf_ret = my_scanf(format_str, my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%s'\n", my_scanf_ret, my_scanf_val);

    // Check results
    int values_match = (strcmp(scanf_val, my_scanf_val) == 0);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (returns_match && values_match && scanf_ret == 1) {
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

int test_scanf_float(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    float scanf_val = -999.0f;
    int scanf_ret = scanf("%f", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %f\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    float my_scanf_val = -999.0f;
    int my_scanf_ret = my_scanf("%f", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %f\n", my_scanf_ret, my_scanf_val);

    // Check behavior - use small epsilon for float comparison
    float epsilon = 0.0001f;
    float diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS) {
        if (returns_match && values_match && scanf_ret == 1) {
            printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    } else {
        if (scanf_ret == 0 && my_scanf_ret == 0) {
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    }

    printf("***\n");
    return passed;
}

int test_scanf_float_with_format(const char *test_name, const char *input_file, const char *format_str, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    float scanf_val = -999.0f;
    int scanf_ret = scanf(format_str, &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %f\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    float my_scanf_val = -999.0f;
    int my_scanf_ret = my_scanf(format_str, &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %f\n", my_scanf_ret, my_scanf_val);

    // Check behavior - use small epsilon for float comparison
    float epsilon = 0.0001f;
    float diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS) {
        if (returns_match && values_match && scanf_ret == 1) {
            printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    } else {
        if (scanf_ret == 0 && my_scanf_ret == 0) {
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    }

    printf("***\n");
    return passed;
}

int test_scanf_double(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    double scanf_val = -999.0;
    int scanf_ret = scanf("%lf", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %.15lf\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    double my_scanf_val = -999.0;
    int my_scanf_ret = my_scanf("%lf", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %.15lf\n", my_scanf_ret, my_scanf_val);

    // Check behavior - use small epsilon for double comparison
    double epsilon = 0.0000001;
    double diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS) {
        if (returns_match && values_match && scanf_ret == 1) {
            printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    } else {
        if (scanf_ret == 0 && my_scanf_ret == 0) {
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    }

    printf("***\n");
    return passed;
}

int test_scanf_long_double(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long double scanf_val = -999.0L;
    int scanf_ret = scanf("%Lf", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %.18Lf\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    long double my_scanf_val = -999.0L;
    int my_scanf_ret = my_scanf("%Lf", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %.18Lf\n", my_scanf_ret, my_scanf_val);

    // Check behavior - use small epsilon for long double comparison
    long double epsilon = 0.000000001L;
    long double diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("Expected: %s\n", expected == EXPECT_SUCCESS ? "SUCCESS" : "FAILURE");
    printf("Behavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS) {
        if (returns_match && values_match && scanf_ret == 1) {
            printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    } else {
        if (scanf_ret == 0 && my_scanf_ret == 0) {
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            passed = 1;
            tests_passed++;
        } else {
            printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
            tests_failed++;
        }
    }

    printf("***\n");
    return passed;
}

int test_scanf_int_with_format(const char *test_name, const char *input_file, const char *format_str, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val = -999;
    int scanf_ret = scanf(format_str, &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val = -999;
    int my_scanf_ret = my_scanf(format_str, &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

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

int test_scanf_hex(const char *test_name, const char *input_file, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val = -999;
    int scanf_ret = scanf("%x", &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %x (%d)\n", scanf_ret, scanf_val, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val = -999;
    int my_scanf_ret = my_scanf("%x", &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %x (%d)\n", my_scanf_ret, my_scanf_val, my_scanf_val);

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

int test_scanf_hex_with_format(const char *test_name, const char *input_file, const char *format_str, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s \n", test_name);
    printf("Input file: %s\n", input_file);
    printf("Format: %s\n", format_str);

    // Test scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not open input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int scanf_val = -999;
    int scanf_ret = scanf(format_str, &scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %x (%d)\n", scanf_ret, scanf_val, scanf_val);

    // Test my_scanf()
    stdin = freopen(input_file, "r", stdin);
    if (!stdin) {
        printf("%sFAIL: Could not reopen input file\n%s", COLOR_RED, COLOR_RESET);
        tests_failed++;
        return 0;
    }

    int my_scanf_val = -999;
    int my_scanf_ret = my_scanf(format_str, &my_scanf_val);

    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %x (%d)\n", my_scanf_ret, my_scanf_val, my_scanf_val);

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

    printf("\n=== %%ld Tests =========================\n");
    test_scanf_long("Long integer", "test_inputs/test_long.txt", EXPECT_SUCCESS);
    test_scanf_long("Negative long", "test_inputs/test_long_neg.txt", EXPECT_SUCCESS);
    test_scanf_long("LONG_MAX", "test_inputs/test_long_max.txt", EXPECT_SUCCESS);

    printf("\n=== %%lld Tests ========================\n");
    test_scanf_long_long("Long long max", "test_inputs/test_llong.txt", EXPECT_SUCCESS);
    test_scanf_long_long("Long long min", "test_inputs/test_llong_neg.txt", EXPECT_SUCCESS);
    test_scanf_long_long("Large long long", "test_inputs/test_llong_large.txt", EXPECT_SUCCESS);

    printf("\n=== %%hd Tests =========================\n");
    test_scanf_short("Short max", "test_inputs/test_short.txt", EXPECT_SUCCESS);
    test_scanf_short("Short min", "test_inputs/test_short_neg.txt", EXPECT_SUCCESS);
    test_scanf_short("Small short", "test_inputs/test_short_small.txt", EXPECT_SUCCESS);

    printf("\n=== %%d Field Width Tests ==============\n");
    test_scanf_int_with_format("Int field width 3", "test_inputs/test_int_width3.txt", "%3d", EXPECT_SUCCESS);
    test_scanf_int_with_format("Int field width 5 with sign", "test_inputs/test_int_width5.txt", "%5d", EXPECT_SUCCESS);

    printf("\n=== %%c Tests ==========================\n");
    test_scanf_char("One character", "test_inputs/test_char_a.txt", EXPECT_SUCCESS);
    test_scanf_char("Space", "test_inputs/test_char_space.txt", EXPECT_SUCCESS);
    test_scanf_char("Digit", "test_inputs/test_char_digit.txt", EXPECT_SUCCESS);

    printf("\n=== %%c Field Width Tests ==============\n");
    test_scanf_char_multi("Read 3 chars", "test_inputs/test_char_multi.txt", "%3c", 3, EXPECT_SUCCESS);
    test_scanf_char_multi("Read 5 chars", "test_inputs/test_char_multi.txt", "%5c", 5, EXPECT_SUCCESS);
    test_scanf_char_multi("Read 3 chars with spaces", "test_inputs/test_char_with_space.txt", "%3c", 3, EXPECT_SUCCESS);

    printf("\n=== %%s Tests ==========================\n");
    test_scanf_string("Simple string", "test_inputs/test_string.txt", EXPECT_SUCCESS);
    test_scanf_string("String with leading space", "test_inputs/test_string_space.txt", EXPECT_SUCCESS);

    printf("\n=== %%d %%c Tests ======================\n");
    test_scanf_int_char("Integer and character", "test_inputs/test_d_then_c.txt", EXPECT_SUCCESS);

    printf("\n=== Assignment Suppression Tests =======\n");
    // %d %*d %d with "10 20 30" - scanf determines correct behavior
    test_scanf_suppress_int("Suppress middle int", "test_inputs/test_suppress_int.txt","%d %*d %d");
    // %*d %d with "10 20 30" - scanf determines correct behavior
    test_scanf_suppress_int("Suppress first int", "test_inputs/test_suppress_int.txt","%*d %d");
    // %*c%c with "abc" - scanf determines correct behavior
    test_scanf_suppress_char("Suppress first char", "test_inputs/test_suppress_char.txt", "%*c%c");

    printf("\n=== %%s Field Width Tests ==============\n");
    test_scanf_string_with_format("String field width 5", "test_inputs/test_string_field_width.txt","%5s");
    test_scanf_string_with_format("String field width 10", "test_inputs/test_string_field_width.txt", "%10s");
    test_scanf_string_with_format("String field width with spaces", "test_inputs/test_string_field_width_space.txt", "%5s");
    test_scanf_string_suppress("Suppress string", "test_inputs/test_string_suppress.txt","%*s %s");

    printf("\n=== %%f Tests ==========================\n");
    test_scanf_float("Positive float", "test_inputs/test_float.txt", EXPECT_SUCCESS);
    test_scanf_float("Negative float", "test_inputs/test_float_neg.txt", EXPECT_SUCCESS);
    test_scanf_float("Integer as float", "test_inputs/test_float_no_decimal.txt", EXPECT_SUCCESS);
    test_scanf_float("Float with leading space", "test_inputs/test_float_leading_space.txt", EXPECT_SUCCESS);
    test_scanf_float("Zero float", "test_inputs/test_float_zero.txt", EXPECT_SUCCESS);
    test_scanf_float("Negative zero float", "test_inputs/test_float_neg_zero.txt", EXPECT_SUCCESS);
    test_scanf_float_with_format("Float field width 5", "test_inputs/test_float_width5.txt", "%5f", EXPECT_SUCCESS);
    test_scanf_float_with_format("Float field width 3", "test_inputs/test_float_width3.txt", "%3f", EXPECT_SUCCESS);

    printf("\n=== %%lf Tests =========================\n");
    test_scanf_double("Double precision", "test_inputs/test_double.txt", EXPECT_SUCCESS);

    printf("\n=== %%Lf Tests =========================\n");
    test_scanf_long_double("Long double", "test_inputs/test_long_double.txt", EXPECT_SUCCESS);

    printf("\n=== %%x Tests ==========================\n");
    test_scanf_hex("Hex lowercase", "test_inputs/test_hex.txt", EXPECT_SUCCESS);
    test_scanf_hex("Hex with 0x prefix", "test_inputs/test_hex_prefix.txt", EXPECT_SUCCESS);
    test_scanf_hex("Hex uppercase", "test_inputs/test_hex_upper.txt", EXPECT_SUCCESS);
    test_scanf_hex("Hex zero", "test_inputs/test_hex_zero.txt", EXPECT_SUCCESS);
    test_scanf_hex("Hex with leading space", "test_inputs/test_hex_leading_space.txt", EXPECT_SUCCESS);

    printf("\n=== %%x Field Width Tests ==============\n");
    test_scanf_hex_with_format("Hex field width 3", "test_inputs/test_hex_width3.txt", "%3x", EXPECT_SUCCESS);
    test_scanf_hex_with_format("Hex field width 4 with prefix", "test_inputs/test_hex_width4.txt", "%4x", EXPECT_SUCCESS);

    // Print summary
    printf("\n========================================\n");
    printf("TEST SUMMARY\n");
    printf("  Tests run:    %d\n", tests_run);
    printf("  %sPassed:       %d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("  %sFailed:       %d%s\n", COLOR_RED, tests_failed, COLOR_RESET);
    printf("========================================\n");


    return (tests_failed == 0) ? 0 : 1;
}