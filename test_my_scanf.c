#include "my_scanf.h"
#include <stdio.h>
#include <string.h>

/*
    About the testing structure:

    To mirror the interaction between scanf() and the buffer as closely as
    possible, each test reads from small input files that are redirected to
    stdin. Both scanf() and my_scanf() process the same input file separately,
    and their behavior (return values and parsed values) is compared.

    Test categories:
    - Standard specifiers (%d, %f, %s, %x, etc.): Compare directly to scanf()
    - Custom specifiers (%b, %z, %q): Cannot compare to scanf(); instead verify
      against expected hardcoded values or use round-trip validation (e.g.,
      cipher→decipher→original)
    - Combo tests with custom specifiers: Use specialized test functions that
      don't call scanf(), since scanf() doesn't understand custom specifiers
*/

// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RESET "\033[0m"

typedef enum { EXPECT_SUCCESS, EXPECT_FAILURE } ExpectedBehavior;

// GENERIC TEST HELPERS
// check_..._match functions: Compare scanf() and my_scanf() results for
// different data types (int, float, double, string). Each function checks:
//   - Return values match
//   - Parsed values match (within epsilon for floats/doubles)
//   - Both functions exhibit the expected behavior (success or failure)
int check_int_match(int scanf_ret, int my_scanf_ret, int scanf_val, int my_scanf_val, ExpectedBehavior expected) {
    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret <= 0 && my_scanf_ret <= 0);
    int values_match = (scanf_val == my_scanf_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    if (expected == EXPECT_SUCCESS) {
        if (both_succeeded && values_match && returns_match) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both succeeded with matching values)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (scanf: ret=%d val=%d, my_scanf: ret=%d val=%d)\n",
                   COLOR_RED, COLOR_RESET, scanf_ret, scanf_val, my_scanf_ret, my_scanf_val);
            return 0;
        }
    } else {
        if (both_failed) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (one succeeded when should fail)\n", COLOR_RED, COLOR_RESET);
            return 0;
        }
    }
}

int check_float_match(int scanf_ret, int my_scanf_ret, float scanf_val, float my_scanf_val, ExpectedBehavior expected) {
    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret <= 0 && my_scanf_ret <= 0);
    float epsilon = 0.0001f;
    float diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    if (expected == EXPECT_SUCCESS) {
        if (both_succeeded && values_match && returns_match) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both succeeded with matching values)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (scanf: ret=%d val=%f, my_scanf: ret=%d val=%f)\n",
                   COLOR_RED, COLOR_RESET, scanf_ret, scanf_val, my_scanf_ret, my_scanf_val);
            return 0;
        }
    } else {
        if (both_failed) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (one succeeded when should fail)\n", COLOR_RED, COLOR_RESET);
            return 0;
        }
    }
}

int check_double_match(int scanf_ret, int my_scanf_ret, double scanf_val, double my_scanf_val, ExpectedBehavior expected) {
    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret <= 0 && my_scanf_ret <= 0);
    double epsilon = 0.0000001;
    double diff = scanf_val - my_scanf_val;
    if (diff < 0) diff = -diff;
    int values_match = (diff < epsilon);
    int returns_match = (scanf_ret == my_scanf_ret);

    if (expected == EXPECT_SUCCESS) {
        if (both_succeeded && values_match && returns_match) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both succeeded with matching values)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (scanf: ret=%d val=%.15lf, my_scanf: ret=%d val=%.15lf)\n",
                   COLOR_RED, COLOR_RESET, scanf_ret, scanf_val, my_scanf_ret, my_scanf_val);
            return 0;
        }
    } else {
        if (both_failed) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (one succeeded when should fail)\n", COLOR_RED, COLOR_RESET);
            return 0;
        }
    }
}

int check_string_match(int scanf_ret, int my_scanf_ret, const char *scanf_val, const char *my_scanf_val, ExpectedBehavior expected) {
    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int both_failed = (scanf_ret <= 0 && my_scanf_ret <= 0);
    int values_match = (strcmp(scanf_val, my_scanf_val) == 0);
    int returns_match = (scanf_ret == my_scanf_ret);

    if (expected == EXPECT_SUCCESS) {
        if (both_succeeded && values_match && returns_match) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both succeeded with matching values)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (scanf: ret=%d val='%s', my_scanf: ret=%d val='%s')\n",
                   COLOR_RED, COLOR_RESET, scanf_ret, scanf_val, my_scanf_ret, my_scanf_val);
            return 0;
        }
    } else {
        if (both_failed) {
            printf("\tBehavior match: YES\n");
            printf("Result: %sPASS%s (both failed as expected)\n", COLOR_GREEN, COLOR_RESET);
            return 1;
        } else {
            printf("\tBehavior match: NO\n");
            printf("Result: %sFAIL%s (one succeeded when should fail)\n", COLOR_RED, COLOR_RESET);
            return 0;
        }
    }
}

// Helper for the test_binary function
int manual_binary_to_int(const char *s) {
    int value = 0, sign = 1, i = 0;
    if (s[i] == '-') { sign = -1; i++; } else if (s[i] == '+') { i++; }
    if (s[i] == '0' && (s[i+1] == 'b' || s[i+1] == 'B')) i += 2;
    while (s[i] == '0' || s[i] == '1') value = value * 2 + (s[i++] - '0');
    return value * sign;
}

// GENERIC TEST RUNNERS
// test_... functions: Redirect input files to stdin, call scanf() and
// my_scanf() with the same format, then compare results. Functions are
// specialized by data type. Custom specifier tests only call my_scanf()
// since scanf() doesn't understand %b, %z, or %q.
int test_int(const char *name, const char *file, const char *fmt, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int scanf_val = -999, my_scanf_val = -999;
    int scanf_ret = scanf(fmt, &scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %d\n", scanf_ret, scanf_val);

    stdin = freopen(file, "r", stdin);
    int my_scanf_ret = my_scanf(fmt, &my_scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %d\n", my_scanf_ret, my_scanf_val);

    int passed = check_int_match(scanf_ret, my_scanf_ret, scanf_val, my_scanf_val, expected);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_long(const char *name, const char *file, const char *fmt, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    long scanf_val = -999, my_scanf_val = -999;
    int scanf_ret = scanf(fmt, &scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %ld\n", scanf_ret, scanf_val);

    stdin = freopen(file, "r", stdin);
    int my_scanf_ret = my_scanf(fmt, &my_scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %ld\n", my_scanf_ret, my_scanf_val);

    int both_succeeded = (scanf_ret > 0 && my_scanf_ret > 0);
    int values_match = (scanf_val == my_scanf_val);
    int returns_match = (scanf_ret == my_scanf_ret);

    printf("\tBehavior match: %s\n", (returns_match && values_match) ? "YES" : "NO");

    int passed = 0;
    if (expected == EXPECT_SUCCESS && both_succeeded && values_match && returns_match) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else if (expected == EXPECT_FAILURE && scanf_ret <= 0 && my_scanf_ret <= 0) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }

    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_float(const char *name, const char *file, const char *fmt, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    float scanf_val = -999.0f, my_scanf_val = -999.0f;
    int scanf_ret = scanf(fmt, &scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %f\n", scanf_ret, scanf_val);

    stdin = freopen(file, "r", stdin);
    int my_scanf_ret = my_scanf(fmt, &my_scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %f\n", my_scanf_ret, my_scanf_val);

    int passed = check_float_match(scanf_ret, my_scanf_ret, scanf_val, my_scanf_val, expected);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_double(const char *name, const char *file, const char *fmt, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    double scanf_val = -999.0, my_scanf_val = -999.0;
    int scanf_ret = scanf(fmt, &scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: %.15lf\n", scanf_ret, scanf_val);

    stdin = freopen(file, "r", stdin);
    int my_scanf_ret = my_scanf(fmt, &my_scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: %.15lf\n", my_scanf_ret, my_scanf_val);

    int passed = check_double_match(scanf_ret, my_scanf_ret, scanf_val, my_scanf_val, expected);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_string(const char *name, const char *file, const char *fmt, ExpectedBehavior expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    char scanf_val[256] = {0}, my_scanf_val[256] = {0};
    int scanf_ret = scanf(fmt, scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, value: '%s'\n", scanf_ret, scanf_val);

    stdin = freopen(file, "r", stdin);
    int my_scanf_ret = my_scanf(fmt, my_scanf_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%s'\n", my_scanf_ret, my_scanf_val);

    int passed = check_string_match(scanf_ret, my_scanf_ret, scanf_val, my_scanf_val, expected);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_multiple_ints(const char *name, const char *file, const char *fmt, int expected_count) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999, v3 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2, &v3);
    freopen("/dev/tty", "r", stdin);

    printf("scanf()    returned: %d, values: %d %d %d\n", scanf_ret, v1, v2, v3);

    stdin = freopen(file, "r", stdin);
    int v4 = -999, v5 = -999, v6 = -999;
    int my_scanf_ret = my_scanf(fmt, &v4, &v5, &v6);
    freopen("/dev/tty", "r", stdin);

    printf("my_scanf() returned: %d, values: %d %d %d\n", my_scanf_ret, v4, v5, v6);

    int passed = 0;
    if (scanf_ret == my_scanf_ret && scanf_ret == expected_count && v1 == v4 && v2 == v5 && v3 == v6) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_two_ints_with_literal(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2);
    freopen("/dev/tty", "r", stdin);

    printf("scanf()    returned: %d, values: %d %d\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    int v3 = -999, v4 = -999;
    int my_scanf_ret = my_scanf(fmt, &v3, &v4);
    freopen("/dev/tty", "r", stdin);

    printf("my_scanf() returned: %d, values: %d %d\n", my_scanf_ret, v3, v4);

    int passed = 0;
    if (scanf_ret == my_scanf_ret && v1 == v3 && v2 == v4) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_binary(const char *name, const char *file) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int my_val = -999;
    int my_ret = my_scanf("%b", &my_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf(%%b) returned: %d, value: %d\n", my_ret, my_val);

    stdin = freopen(file, "r", stdin);
    char bin_str[100] = {0};
    scanf("%s", bin_str);
    freopen("/dev/tty", "r", stdin);

    int manual_val = manual_binary_to_int(bin_str);
    printf("\tManual conversion of '%s': %d\n", bin_str, manual_val);

    printf("Result: ");
    int passed = 0;
    if (my_ret == 1 && my_val == manual_val) {
        printf("%sPASS%s (all methods agree: %d)\n", COLOR_GREEN, COLOR_RESET, my_val);
        passed = 1;
        tests_passed++;
    } else {
        printf("%sFAIL%s (got %d, expected %d)\n", COLOR_RED, COLOR_RESET, my_val, manual_val);
        tests_failed++;
    }
    printf("***\n");
    return passed;
}

int test_genz(const char *name, const char *file, const char *fmt, const char *expected) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    char my_val[256] = {0};
    int my_ret = my_scanf(fmt, my_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, value: '%s'\n", my_ret, my_val);
    printf("\tExpected: '%s'\n", expected);

    printf("Result: ");
    int passed = 0;
    if (my_ret == 1 && strcmp(my_val, expected) == 0) {
        printf("%sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else {
        printf("%sFAIL%s\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
    }
    printf("***\n");
    return passed;
}

int test_cipher(const char *name, const char *file, int offset) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Cipher offset: %d\n", offset);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    char original[256] = {0};
    fgets(original, sizeof(original), stdin);
    size_t len = strlen(original);
    if (len > 0 && original[len-1] == '\n') original[len-1] = '\0';
    freopen("/dev/tty", "r", stdin);

    printf("\tOriginal text: '%s'\n", original);

    char fmt[10];
    snprintf(fmt, sizeof(fmt), "%%%dq", offset);

    stdin = freopen(file, "r", stdin);
    char ciphered[256] = {0};
    my_scanf(fmt, ciphered);
    freopen("/dev/tty", "r", stdin);

    printf("\tCiphered text: '%s'\n", ciphered);

    FILE *temp = fopen("test_inputs/temp_cipher.txt", "w");
    fprintf(temp, "%s\n", ciphered);
    fclose(temp);

    int reverse_offset = (26 - offset) % 26;
    snprintf(fmt, sizeof(fmt), "%%%dq", reverse_offset);

    stdin = freopen("test_inputs/temp_cipher.txt", "r", stdin);
    char deciphered[256] = {0};
    my_scanf(fmt, deciphered);
    freopen("/dev/tty", "r", stdin);
    remove("test_inputs/temp_cipher.txt");

    printf("\tDeciphered text: '%s'\n", deciphered);

    printf("Result: ");
    int passed = 0;
    if (strcmp(original, deciphered) == 0) {
        printf("%sPASS%s (cipher → decipher matches original)\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else {
        printf("%sFAIL%s\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
    }
    printf("***\n");
    return passed;
}

int test_suppress_two(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %d\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    int v3 = -999, v4 = -999;
    int my_scanf_ret = my_scanf(fmt, &v3, &v4);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %d\n", my_scanf_ret, v3, v4);

    int passed = 0;
    if (scanf_ret == my_scanf_ret && v1 == v3 && v2 == v4) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_suppress_three(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999, v3 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2, &v3);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %d %d\n", scanf_ret, v1, v2, v3);

    stdin = freopen(file, "r", stdin);
    int v4 = -999, v5 = -999, v6 = -999;
    int my_scanf_ret = my_scanf(fmt, &v4, &v5, &v6);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %d %d\n", my_scanf_ret, v4, v5, v6);

    int passed = 0;
    if (scanf_ret == my_scanf_ret && v1 == v4 && v2 == v5 && v3 == v6) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_suppress_float(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    float v1 = -999.0f, v2 = -999.0f;
    int scanf_ret = scanf(fmt, &v1, &v2);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %f %f\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    float v3 = -999.0f, v4 = -999.0f;
    int my_scanf_ret = my_scanf(fmt, &v3, &v4);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %f %f\n", my_scanf_ret, v3, v4);

    int values_match = 1;
    if (scanf_ret > 0 && my_scanf_ret > 0) {
        float epsilon = 0.0001f;
        float diff1 = v1 - v3; if (diff1 < 0) diff1 = -diff1;
        float diff2 = v2 - v4; if (diff2 < 0) diff2 = -diff2;
        values_match = (diff1 < epsilon && diff2 < epsilon);
    } else {
        values_match = (v1 == v3 && v2 == v4);
    }

    int passed = 0;
    if (scanf_ret == my_scanf_ret && values_match) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_suppress_string(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    char v1[256] = {0}, v2[256] = {0};
    int scanf_ret = scanf(fmt, v1, v2);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: '%s' '%s'\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    char v3[256] = {0}, v4[256] = {0};
    int my_scanf_ret = my_scanf(fmt, v3, v4);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: '%s' '%s'\n", my_scanf_ret, v3, v4);

    int passed = 0;
    if (scanf_ret == my_scanf_ret && strcmp(v1, v3) == 0 && strcmp(v2, v4) == 0) {
        printf("Result: %sPASS%s\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
    } else {
        printf("Result: %sFAIL%s\n", COLOR_RED, COLOR_RESET);
    }
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_combo_two_ints(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %d\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    int v3 = -999, v4 = -999;
    int my_scanf_ret = my_scanf(fmt, &v3, &v4);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %d\n", my_scanf_ret, v3, v4);

    int passed = (scanf_ret == my_scanf_ret && v1 == v3 && v2 == v4);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED, passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_combo_three_mixed(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int i_val = -999;
    float f_val = -999.0f;
    char s_val[256] = {0};
    int scanf_ret = scanf(fmt, &i_val, &f_val, s_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %f '%s'\n", scanf_ret, i_val, f_val, s_val);

    stdin = freopen(file, "r", stdin);
    int m_i_val = -999;
    float m_f_val = -999.0f;
    char m_s_val[256] = {0};
    int my_scanf_ret = my_scanf(fmt, &m_i_val, &m_f_val, m_s_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %f '%s'\n", my_scanf_ret, m_i_val, m_f_val, m_s_val);

    float epsilon = 0.0001f;
    float diff = f_val - m_f_val;
    if (diff < 0) diff = -diff;
    int passed = (scanf_ret == my_scanf_ret && i_val == m_i_val && diff < epsilon && strcmp(s_val, m_s_val) == 0);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED, passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_combo_binary_genz_int(const char *name, const char *file, const char *fmt) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int b_val = -999;
    char z_val[256] = {0};
    int i_val = -999;
    int scanf_ret = scanf(fmt, &b_val, z_val, &i_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d '%s' %d\n", scanf_ret, b_val, z_val, i_val);

    stdin = freopen(file, "r", stdin);
    int m_b_val = -999;
    char m_z_val[256] = {0};
    int m_i_val = -999;
    int my_scanf_ret = my_scanf(fmt, &m_b_val, m_z_val, &m_i_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d '%s' %d\n", my_scanf_ret, m_b_val, m_z_val, m_i_val);

    int passed = (scanf_ret == my_scanf_ret && b_val == m_b_val && strcmp(z_val, m_z_val) == 0 && i_val == m_i_val);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED, passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_cipher_inverted(const char *name, const char *file, int offset) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Cipher offset: %d with case inversion\n", offset);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    char original[256] = {0};
    fgets(original, sizeof(original), stdin);
    size_t len = strlen(original);
    if (len > 0 && original[len-1] == '\n') original[len-1] = '\0';
    freopen("/dev/tty", "r", stdin);

    printf("\tOriginal text: '%s'\n", original);

    // First pass: cipher with offset and case inversion
    char fmt[10];
    snprintf(fmt, sizeof(fmt), "%%%dq!", offset);

    stdin = freopen(file, "r", stdin);
    char ciphered[256] = {0};
    my_scanf(fmt, ciphered);
    freopen("/dev/tty", "r", stdin);

    printf("\tCiphered text (inverted): '%s'\n", ciphered);

    FILE *temp = fopen("test_inputs/temp_cipher.txt", "w");
    fprintf(temp, "%s\n", ciphered);
    fclose(temp);

    // Second pass: decipher with opposite offset and SAME case inversion
    int reverse_offset = (26 - offset) % 26;
    snprintf(fmt, sizeof(fmt), "%%%dq!", reverse_offset);

    stdin = freopen("test_inputs/temp_cipher.txt", "r", stdin);
    char deciphered[256] = {0};
    my_scanf(fmt, deciphered);
    freopen("/dev/tty", "r", stdin);
    remove("test_inputs/temp_cipher.txt");

    printf("\tDeciphered text: '%s'\n", deciphered);

    printf("Result: ");
    int passed = 0;
    if (strcmp(original, deciphered) == 0) {
        printf("%sPASS%s (cipher → decipher with inversion matches original)\n", COLOR_GREEN, COLOR_RESET);
        passed = 1;
        tests_passed++;
    } else {
        printf("%sFAIL%s\n", COLOR_RED, COLOR_RESET);
        tests_failed++;
    }
    printf("***\n");
    return passed;
}

int test_combo_custom_only(const char *name, const char *file, const char *fmt, int expected_ret, int exp_val1, const char *exp_val2, int exp_val3) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999;
    char v2[256] = {0};
    int v3 = -999;
    int my_scanf_ret = my_scanf(fmt, &v1, v2, &v3);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d '%s' %d\n", my_scanf_ret, v1, v2, v3);
    printf("\tExpected: ret=%d, values: %d '%s' %d\n", expected_ret, exp_val1, exp_val2, exp_val3);

    int passed = (my_scanf_ret == expected_ret && v1 == exp_val1 &&
                  strcmp(v2, exp_val2) == 0 && v3 == exp_val3);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED,
           passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_combo_binary_native(const char *name, const char *file, const char *fmt, int expected_ret, int exp_bin, int exp_int) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int b_val = -999;
    int i_val = -999;
    int my_scanf_ret = my_scanf(fmt, &b_val, &i_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, binary: %d, int: %d\n", my_scanf_ret, b_val, i_val);
    printf("\tExpected: ret=%d, binary: %d, int: %d\n", expected_ret, exp_bin, exp_int);

    int passed = (my_scanf_ret == expected_ret && b_val == exp_bin && i_val == exp_int);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED,
           passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_combo_int_genz(const char *name, const char *file, const char *fmt, int expected_ret, int exp_int, const char *exp_genz) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int i_val = -999;
    char z_val[256] = {0};
    int my_scanf_ret = my_scanf(fmt, &i_val, z_val);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, int: %d, genz: '%s'\n", my_scanf_ret, i_val, z_val);
    printf("\tExpected: ret=%d, int: %d, genz: '%s'\n", expected_ret, exp_int, exp_genz);

    int passed = (my_scanf_ret == expected_ret && i_val == exp_int && strcmp(z_val, exp_genz) == 0);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED,
           passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

int test_literal_percent(const char *name, const char *file, const char *fmt, int expected_ret, int exp_val1, int exp_val2) {
    tests_run++;
    printf("\nTEST: %s\n", name);
    printf("Input file: %s\n", file);
    printf("Format: %s\n", fmt);

    stdin = freopen(file, "r", stdin);
    if (!stdin) { printf("FAIL: Can't open %s\n", file); tests_failed++; return 0; }

    int v1 = -999, v2 = -999;
    int scanf_ret = scanf(fmt, &v1, &v2);
    freopen("/dev/tty", "r", stdin);

    printf("\tscanf()    returned: %d, values: %d %d\n", scanf_ret, v1, v2);

    stdin = freopen(file, "r", stdin);
    int v3 = -999, v4 = -999;
    int my_scanf_ret = my_scanf(fmt, &v3, &v4);
    freopen("/dev/tty", "r", stdin);

    printf("\tmy_scanf() returned: %d, values: %d %d\n", my_scanf_ret, v3, v4);
    printf("\tExpected: ret=%d, values: %d %d\n", expected_ret, exp_val1, exp_val2);

    int passed = (scanf_ret == my_scanf_ret && my_scanf_ret == expected_ret &&
                  v1 == v3 && v2 == v4 && v1 == exp_val1 && v2 == exp_val2);
    printf("Result: %s%s%s\n", passed ? COLOR_GREEN : COLOR_RED,
           passed ? "PASS" : "FAIL", COLOR_RESET);
    printf("***\n");
    if (passed) tests_passed++; else tests_failed++;
    return passed;
}

// MAIN TEST SUITE
int main() {
    printf("\n=== MY_SCANF TEST SUITE ===\n\n");

    printf("--- INTEGER BASICS (%%d) ---\n");
    test_int("Positive integer", "test_inputs/test_pos.txt", "%d", EXPECT_SUCCESS);
    test_int("Negative integer", "test_inputs/test_neg.txt", "%d", EXPECT_SUCCESS);
    test_int("Zero", "test_inputs/test_zero.txt", "%d", EXPECT_SUCCESS);
    test_int("INT_MAX", "test_inputs/test_int_max.txt", "%d", EXPECT_SUCCESS);
    test_int("INT_MIN", "test_inputs/test_int_min.txt", "%d", EXPECT_SUCCESS);
    test_int("Plus sign", "test_inputs/test_plus_sign.txt", "%d", EXPECT_SUCCESS);
    test_int("Leading spaces", "test_inputs/test_leading_space.txt", "%d", EXPECT_SUCCESS);
    test_int("Decimal point (reads partial)", "test_inputs/test_decimal.txt", "%d", EXPECT_SUCCESS);
    test_int("Trailing letter", "test_inputs/test_trailing_letter.txt", "%d", EXPECT_SUCCESS);
    test_int("Letters only", "test_inputs/test_letters.txt", "%d", EXPECT_FAILURE);
    test_int("Empty input", "test_inputs/test_empty.txt", "%d", EXPECT_FAILURE);

    printf("\n--- INTEGER OVERFLOW/UNDERFLOW ---\n");
    test_int("Overflow INT_MAX+1", "test_inputs/test_int_overflow.txt", "%d", EXPECT_SUCCESS);
    test_int("Underflow INT_MIN-1", "test_inputs/test_int_underflow.txt", "%d", EXPECT_SUCCESS);

    printf("\n--- INTEGER FIELD WIDTH ---\n");
    test_int("Field width 3", "test_inputs/test_int_width3.txt", "%3d", EXPECT_SUCCESS);
    test_int("Field width 5", "test_inputs/test_int_width5.txt", "%5d", EXPECT_SUCCESS);
    test_int("Field width 0", "test_inputs/test_int_width_0.txt", "%0d", EXPECT_SUCCESS);
    test_int("Field width 1 on multi-digit", "test_inputs/test_int_width_1_multidigit.txt", "%1d", EXPECT_SUCCESS);

    printf("\n--- INTEGER SIGN HANDLING ---\n");
    test_int("Double plus sign", "test_inputs/test_int_double_sign.txt", "%d", EXPECT_FAILURE);
    test_int("Sign with space", "test_inputs/test_int_sign_space.txt", "%d", EXPECT_FAILURE);

    printf("\n--- LONG INTEGERS (%%ld, %%lld, %%hd) ---\n");
    test_long("Long integer", "test_inputs/test_long.txt", "%ld", EXPECT_SUCCESS);
    test_long("Negative long", "test_inputs/test_long_neg.txt", "%ld", EXPECT_SUCCESS);
    test_long("LONG_MAX", "test_inputs/test_long_max.txt", "%ld", EXPECT_SUCCESS);
    test_long("Long long max", "test_inputs/test_llong.txt", "%lld", EXPECT_SUCCESS);
    test_long("Long long min", "test_inputs/test_llong_neg.txt", "%lld", EXPECT_SUCCESS);
    test_long("Short max", "test_inputs/test_short.txt", "%hd", EXPECT_SUCCESS);
    test_long("Short min", "test_inputs/test_short_neg.txt", "%hd", EXPECT_SUCCESS);

    printf("\n--- MULTIPLE INTEGERS ---\n");
    test_multiple_ints("Two integers", "test_inputs/test_two_ints.txt", "%d %d", 2);
    test_multiple_ints("Three integers", "test_inputs/test_three_ints.txt", "%d %d %d", 3);
    test_multiple_ints("Partial success (format longer than input)", "test_inputs/test_partial_success.txt", "%d %d", 1);

    printf("\n--- FLOATS (%%f, %%lf, %%Lf) ---\n");
    test_float("Positive float", "test_inputs/test_float.txt", "%f", EXPECT_SUCCESS);
    test_float("Negative float", "test_inputs/test_float_neg.txt", "%f", EXPECT_SUCCESS);
    test_float("Integer as float", "test_inputs/test_float_no_decimal.txt", "%f", EXPECT_SUCCESS);
    test_float("Zero float", "test_inputs/test_float_zero.txt", "%f", EXPECT_SUCCESS);
    test_float("Field width 5", "test_inputs/test_float_width5.txt", "%5f", EXPECT_SUCCESS);

    printf("\n--- FLOAT SCIENTIFIC NOTATION ---\n");
    test_float("Scientific positive (1.23e5)", "test_inputs/test_float_sci_positive.txt", "%f", EXPECT_SUCCESS);
    test_float("Scientific negative (1.5e-3)", "test_inputs/test_float_sci_negative.txt", "%f", EXPECT_SUCCESS);
    test_float("Scientific capital E", "test_inputs/test_float_sci_capital.txt", "%f", EXPECT_SUCCESS);

    printf("\n--- FLOAT DECIMAL EDGE CASES ---\n");
    test_float("Leading decimal (.5)", "test_inputs/test_float_leading_decimal.txt", "%f", EXPECT_SUCCESS);
    test_float("Trailing decimal (5.)", "test_inputs/test_float_trailing_decimal.txt", "%f", EXPECT_SUCCESS);

    printf("\n--- DOUBLE PRECISION ---\n");
    test_double("Double precision", "test_inputs/test_double.txt", "%lf", EXPECT_SUCCESS);
    test_double("Long double", "test_inputs/test_long_double.txt", "%Lf", EXPECT_SUCCESS);

    printf("\n--- HEXADECIMAL (%%x) ---\n");
    test_int("Hex lowercase", "test_inputs/test_hex.txt", "%x", EXPECT_SUCCESS);
    test_int("Hex with prefix", "test_inputs/test_hex_prefix.txt", "%x", EXPECT_SUCCESS);
    test_int("Hex uppercase", "test_inputs/test_hex_upper.txt", "%x", EXPECT_SUCCESS);
    test_int("Hex zero", "test_inputs/test_hex_zero.txt", "%x", EXPECT_SUCCESS);
    test_int("Hex field width 3", "test_inputs/test_hex_width3.txt", "%3x", EXPECT_SUCCESS);
    test_int("Hex negative", "test_inputs/test_hex_negative.txt", "%x", EXPECT_SUCCESS);

    printf("\n--- CHARACTER & STRING ---\n");
    test_string("Single character", "test_inputs/test_char_a.txt", "%c", EXPECT_SUCCESS);
    test_string("Simple string", "test_inputs/test_string.txt", "%s", EXPECT_SUCCESS);
    test_string("String with leading space", "test_inputs/test_string_space.txt", "%s", EXPECT_SUCCESS);
    test_string("String field width 5", "test_inputs/test_string_field_width.txt", "%5s", EXPECT_SUCCESS);
    test_string("Empty input string", "test_inputs/test_string_empty.txt", "%s", EXPECT_FAILURE);
    test_string("Only whitespace input", "test_inputs/test_int_only_spaces.txt", "%s", EXPECT_FAILURE);

    printf("\n--- STRING LITERAL MATCHING ---\n");
    test_two_ints_with_literal("Literal comma (42,99)", "test_inputs/test_literal_comma.txt", "%d,%d");
    test_two_ints_with_literal("Literal mismatch (expect comma, got semicolon)", "test_inputs/test_literal_mismatch.txt", "%d,%d");

    printf("\n--- CUSTOM %%b (BINARY) ---\n");
    test_binary("Binary 1010", "test_inputs/test_binary_simple.txt");
    test_binary("Binary with prefix", "test_inputs/test_binary_with_prefix.txt");
    test_binary("Binary zero", "test_inputs/test_binary_zero.txt");
    test_binary("Binary negative", "test_inputs/test_binary_negative.txt");
    test_binary("Binary 8 bits", "test_inputs/test_binary_eight_bits.txt");
    test_binary("Binary 32 bits", "test_inputs/test_binary_large.txt");

    printf("\n--- CUSTOM %%z (GEN Z) ---\n");
    test_genz("Simple gen z", "test_inputs/test_genz_simple.txt", "%z", "hello world lol");
    test_genz("Gen z with whitespace", "test_inputs/test_genz_whitespace.txt", "%z", "there's lots of leading and trailing whitespace lol");
    test_genz("Gen z with exclaim", "test_inputs/test_genz_exclaim.txt", "%!z", "that's so cool lol!");
    test_genz("Gen z haha", "test_inputs/test_genz_haha.txt", "%!hz", "this isn't funny haha!");

    printf("\n--- CUSTOM %%q (CIPHER) ---\n");
    test_cipher("Cipher offset 1", "test_inputs/test_cipher_simple.txt", 1);
    test_cipher("Cipher offset 5", "test_inputs/test_cipher_simple.txt", 5);
    test_cipher("Cipher offset 13", "test_inputs/test_cipher_punctuation.txt", 13);
    test_cipher("Cipher wrap around", "test_inputs/test_cipher_wrap.txt", 3);
    test_cipher("Cipher mixed case", "test_inputs/test_cipher_mixed_case.txt", 5);
    test_cipher("Cipher offset 0 (no change)", "test_inputs/test_cipher_simple.txt", 0);
    test_cipher("Cipher offset 26 (full rotation)", "test_inputs/test_cipher_simple.txt", 26);
    test_cipher("Cipher inverted text", "test_inputs/test_cipher_inverted.txt", 3);

    printf("\n--- CUSTOM %%!q (CIPHER WITH CASE INVERSION) ---\n");
    test_cipher_inverted("Cipher inverted offset 5", "test_inputs/test_cipher_simple.txt", 5);
    test_cipher_inverted("Cipher inverted offset 13", "test_inputs/test_cipher_punctuation.txt", 13);
    test_cipher_inverted("Cipher inverted mixed case", "test_inputs/test_cipher_mixed_case.txt", 3);

    printf("\n--- ASSIGNMENT SUPPRESSION (%%*) ---\n");
    test_suppress_two("Suppress first int", "test_inputs/test_suppress_first.txt", "%*d %d");
    test_suppress_three("Suppress middle int", "test_inputs/test_suppress_middle.txt", "%d %*d %d");
    test_suppress_three("Suppress last int", "test_inputs/test_suppress_last.txt", "%d %d %*d");
    test_suppress_three("Suppress all ints", "test_inputs/test_suppress_all.txt", "%*d %*d %*d");
    test_suppress_three("Multiple suppressions", "test_inputs/test_suppress_multiple.txt", "%d %*d %*d %d");
    test_suppress_two("Double suppress then read", "test_inputs/test_suppress_double.txt", "%*d %*d %d");
    test_suppress_two("Suppress with field width", "test_inputs/test_suppress_width.txt", "%*5d %d");
    test_suppress_float("Suppress float", "test_inputs/test_suppress_float.txt", "%*f %f");
    test_suppress_string("Suppress string", "test_inputs/test_suppress_string.txt", "%*s %s");
    test_suppress_two("Suppress hex", "test_inputs/test_suppress_hex.txt", "%*x %x");
    test_suppress_two("Suppress one of two", "test_inputs/test_suppress_one_of_two.txt", "%d %*d");
    test_suppress_two("Suppress with whitespace", "test_inputs/test_suppress_whitespace.txt", "%*d %d");
    test_int("Suppress from empty file", "test_inputs/test_suppress_empty.txt", "%*d", EXPECT_FAILURE);
    test_suppress_two("Suppress then EOF", "test_inputs/test_suppress_eof_early.txt", "%*d %d");

    printf("\n--- COMBO: STANDARD TYPE MIXES ---\n");
    test_suppress_float("Int + Float", "test_inputs/test_combo_int_float.txt", "%d %f");
    test_combo_two_ints("Int + Hex", "test_inputs/test_combo_hex_int.txt", "%x %d");
    test_suppress_string("Int + String", "test_inputs/test_combo_int_string.txt", "%d %s");
    test_suppress_string("Float + String", "test_inputs/test_combo_float_string.txt", "%f %s");

    printf("\n--- COMBO: WITH SUPPRESSION ---\n");
    test_suppress_three("Suppress Hex + Read Ints", "test_inputs/test_combo_suppress_hex_int.txt", "%*x %d %d");
    test_suppress_float("Int + Suppress Float + Float", "test_inputs/test_combo_int_suppress_float.txt", "%d %*f %f");

    printf("\n--- COMBO: MULTIPLE ITEMS (3+) ---\n");
    test_combo_three_mixed("Int + Float + String", "test_inputs/test_combo_int_float_string.txt", "%d %f %s");
    test_combo_three_mixed("Hex + Float + String", "test_inputs/test_combo_hex_float_string.txt", "%x %f %s");

    printf("\n--- COMBO: WITH LITERALS ---\n");
    test_combo_two_ints("Literal Comma", "test_inputs/test_combo_literal_int_comma_int.txt", "%d,%d");
    test_suppress_float("Literal Dash", "test_inputs/test_combo_literal_int_dash_float.txt", "%d-%f");
    test_suppress_two("Literal Colon", "test_inputs/test_combo_literal_hex_colon_int.txt", "%x:%d");

    printf("\n--- COMBO: WHITESPACE HANDLING ---\n");
    test_suppress_three("Whitespace Between Ints", "test_inputs/test_combo_whitespace_int_int.txt", "%d %d %d");
    test_suppress_string("Whitespace Between Strings", "test_inputs/test_combo_whitespace_string_string.txt", "%s %s");

    printf("\n--- COMBO: PARTIAL FAILURES ---\n");
    test_int("First fails", "test_inputs/test_combo_first_fails.txt", "%d", EXPECT_FAILURE);
    test_suppress_two("Middle fails", "test_inputs/test_combo_middle_fails.txt", "%d %d");
    test_suppress_two("Last fails", "test_inputs/test_combo_last_fails.txt", "%d %d");

    printf("\n--- COMBO: RETURN VALUES ---\n");
    test_suppress_float("Read nothing", "test_inputs/test_combo_read_nothing.txt", "%d %f");
    test_suppress_three("All suppressed", "test_inputs/test_combo_all_suppressed.txt", "%*d %*d %*f");
    test_suppress_three("Mixed suppress", "test_inputs/test_combo_mixed_suppress.txt", "%d %*d %f");

    printf("\n--- COMBO: CUSTOM SPECIFIERS ---\n");
    // For test_combo_binary_int.txt containing: "1010 42"
    test_combo_binary_native("Binary + Int", "test_inputs/test_combo_binary_int.txt", "%b %d", 2, 10, 42);
    test_combo_int_genz("Int + Gen Z", "test_inputs/test_combo_int_genz.txt", "%d %z", 2, 123, "hello world lol");
    test_combo_custom_only("Binary + Gen Z + Int", "test_inputs/test_combo_binary_genz_int.txt","%b %z %d", 3, 10, "test lol", 99);

    printf("\n--- %% LITERAL TESTS ---\n");
    test_int("Percent literal: '75%%'","test_inputs/test_percent_simple.txt","%d%%", EXPECT_SUCCESS);
    test_two_ints_with_literal("Percent literal: '10%% 20%%'","test_inputs/test_percent_two.txt","%d%% %d%%");
    test_int("Percent literal partial: missing %%", "test_inputs/test_percent_missing.txt", "%d%%", EXPECT_SUCCESS);
    test_suppress_string("Percent literal: '10%% cheaper'", "test_inputs/test_percent_string.txt", "%d%% %s");

    printf("\n========================================\n");
    printf("TEST SUMMARY\n");
    printf("  Tests run:    %d\n", tests_run);
    printf("  %sPassed:       %d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("  %sFailed:       %d%s\n", COLOR_RED, tests_failed, COLOR_RESET);
    printf("========================================\n\n");

    return (tests_failed == 0) ? 0 : 1;
}