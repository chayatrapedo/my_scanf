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
        return -1;
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

/*
int read_float(float* f) {
    // TO-DO
    // %f - not yet defined
    return 0;
}

int read_hex_integer(int* x) {
    // TO-DO
    // %x - not yet defined
    return 0;
}
*/

int read_char(char* c) {
    int ch = getchar();
    if (ch == EOF) {
        return -1;
    }

    *c = (char)ch;
    return 1;
}

/*
int read_string(char* s, int size) {
    // TO-DO
    // %s - not yet defined
    return 0;
}

// 3 custom modifiers
// reads an unsigned binary integer
int read_unsigned_binary_int(int* b) {
    // TO-DO
    // %b - not yet defined
    return 0;
}

int read_cipher(char* q, int offset) {
    // TO-DO
    // %d - not yet defined
    return 0;
}

// similar to '%s' adds "lol" to the end of every string
int read_gen_z(char* z) {
    // TO-DO
    // %z - not yet defined
    return 0;
}
*/

// - have a sequence that parses the input string to look for multiple values. maybe regex?
int my_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int assigned_count = 0;

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;  // Move this here - increment once before checking specifier
            if (format[i] == '\0') {  // Check if we've hit the end
                break;
            }

            char specifier = format[i];

            switch (specifier) {
                case 'd': {
                    int *ptr = va_arg(args, int*);
                    int result = read_integer(ptr);
                    if (result == 1) {
                        assigned_count++;
                    } else if (result == -1) {
                        // EOF encountered
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    } else {
                        // Conversion failed
                        va_end(args);
                        return assigned_count;
                    }
                    break;  // Remove the i++ from here
                }

                case 'c': {
                    char *ptr = va_arg(args, char*);
                    int result = read_char(ptr);
                    if (result == 1) {
                        assigned_count++;
                    } else if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    break;  // Remove the i++ from here
                }

                default:
                    // Unknown specifier - skip it
                    break;  // Remove the i++ from here
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

/*
int main() {
    // testing bad pointer assignment to scanf()
    int res;
    my_scanf("%c", &res);
    printf("%d",res);
}
*/