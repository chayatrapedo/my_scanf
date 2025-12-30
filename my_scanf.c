#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// helper functions
static void skip_whitespace() {
    int c;
    while ((c = getchar()) != EOF && isspace(c)) {
        // Keep reading until non-whitespace
    }
    if (c != EOF) {
        ungetc(c, stdin);  // Put back the non-whitespace char
    }
}

// helper functions - modifiers
int read_integer(int* d) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Now c is either a non-whitespace char or EOF
    if (c == EOF) {
        return 0;
    }

    // Check for sign
    int sign = 1;
    if (c == '-') {
        sign = -1;
        c = getchar();
    } else if (c == '+') {
        c = getchar();
    }

    int value = 0;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c)) {
        int digit = c - '0';
        value = value * 10 + digit;
        read_any_digits = 1;
        c = getchar();
    }

    // Put back the last character (it wasn't a digit)
    if (c != EOF) {
        ungetc(c, stdin);
    }

    if (read_any_digits) {
        *d = value * sign;
        return 1;
    }

    return 0;
}

// - have a sequence that parses the input string to look for multiple values. maybe regex?
int my_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int assigned_count = 0;

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            char specifier = format[i + 1];

            switch (specifier) {
                case 'd': {
                    int *ptr = va_arg(args, int*);
                    if (read_integer(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;  // Skip the specifier character
                    break;
                }

                default:
                    // Unknown specifier - skip it
                    i++;
                    break;
            }
        } else if (isspace(format[i])) {
            // Format has whitespace - skip whitespace in input
            skip_whitespace();
        } else {
            // Literal character in format - must match input
            int c = getchar();
            if (c != format[i]) {
                // Mismatch - stop processing
                if (c != EOF) {
                    ungetc(c, stdin);
                }
                va_end(args);
                return assigned_count;
            }
        }
    }

    va_end(args);
    return assigned_count;
}