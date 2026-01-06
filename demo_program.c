#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Forward declaration of my_scanf from my_scanf.c
int my_scanf(const char *format, ...);

#define DEMO_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char format_str[DEMO_BUFFER_SIZE];

    printf("Enter a format string: ");
    fgets(format_str, sizeof(format_str), stdin);

    // Remove newline if present
    size_t len = strlen(format_str);
    if (len > 0 && format_str[len - 1] == '\n') {
        format_str[len - 1] = '\0';
    }

    printf("Format string: %s\n", format_str);
    printf("Enter input: ");

    // Allocate space for potential values
    int int_val;
    long long_val;
    float float_val;
    char string_val[DEMO_BUFFER_SIZE];
    char char_val;

    // Try different combinations based on format
    int result = 0;
    if (strstr(format_str, "%d") && strstr(format_str, "%s")) {
        result = my_scanf(format_str, &int_val, string_val);
        if (result >= 1) printf("  int: %d\n", int_val);
        if (result >= 2) printf("  string: %s\n", string_val);
    } else if (strstr(format_str, "%d") && strstr(format_str, "%f")) {
        result = my_scanf(format_str, &int_val, &float_val);
        if (result >= 1) printf("  int: %d\n", int_val);
        if (result >= 2) printf("  float: %f\n", float_val);
    } else if (strstr(format_str, "%ld")) {
        result = my_scanf(format_str, &long_val);
        if (result >= 1) printf("  long: %ld\n", long_val);
    } else if (strstr(format_str, "%f")) {
        result = my_scanf(format_str, &float_val);
        if (result >= 1) printf("  float: %f\n", float_val);
    } else if (strstr(format_str, "%s")) {
        result = my_scanf(format_str, string_val);
        if (result >= 1) printf("  string: %s\n", string_val);
    } else if (strstr(format_str, "%c")) {
        result = my_scanf(format_str, &char_val);
        if (result >= 1) printf("  char: %c\n", char_val);
    } else if (strstr(format_str, "%d")) {
        result = my_scanf(format_str, &int_val);
        if (result >= 1) printf("  int: %d\n", int_val);
    } else if (strstr(format_str, "%z")) {
        result = my_scanf(format_str, string_val);
        if (result >= 1) printf("  result: %s\n", string_val);
    } else if (strstr(format_str, "%b")) {
        result = my_scanf(format_str, &int_val);
        if (result >= 1) printf("  binary: %d\n", int_val);
    } else if (strstr(format_str, "%x")) {
        result = my_scanf(format_str, &int_val);
        if (result >= 1) printf("  hex: %d\n", int_val);
    } else if (strstr(format_str, "%q")) {
        result = my_scanf(format_str, string_val);
        if (result >= 1) printf("  cipher: %s\n", string_val);
    }

    if (result == 0) {
        printf("No valid input found\n");
    } else if (result == -1) {
        printf("EOF encountered\n");
    }

    return 0;
}