#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>


// format specifier structure
typedef struct {
    char specifier;      // 'd', 's', 'c', 'f', 'x', 'z', 'q', 'b'
    int field_width;     // if app. for %31s, this is 31; 0 means no limit
    char length_mod[3];  // "ll", "l", "h", or ""
    int suppress;        // 1 if '*' is present, 0 otherwise
} FormatSpecifier;

// Parse format specifier and return number of characters consumed
// format should point to the character AFTER the '%'
static int parse_format_specifier(const char *format, FormatSpecifier *spec) {
    int pos = 0;

    // Initialize
    spec->field_width = 0;
    spec->length_mod[0] = '\0';
    spec->specifier = '\0';
    spec->suppress = 0;

    // Because there is a set order/pattern for different elements of the modifiers
    // i.e.: characters (*) then numbers then letters
    // they are being parsed in that order

    // Check for assignment suppression '*' first
    if (format[pos] == '*') {
        spec->suppress = 1;
        pos++;
    }

    // Parse field width (digits) next
    while (isdigit(format[pos])) {
        spec->field_width = spec->field_width * 10 + (format[pos] - '0');
        pos++;
    }

    // Parse length modifier (h, l, ll)
    if (format[pos] == 'h') {
        spec->length_mod[0] = 'h';
        spec->length_mod[1] = '\0';
        pos++;
    } else if (format[pos] == 'l') {
        spec->length_mod[0] = 'l';
        if (format[pos + 1] == 'l') {
            spec->length_mod[1] = 'l';
            spec->length_mod[2] = '\0';
            pos += 2;
        } else {
            spec->length_mod[1] = '\0';
            pos++;
        }
    }

    // Parse conversion specifier
    spec->specifier = format[pos];
    pos++;

    return pos;
}

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

// variations of the read integer function
int read_long(long* d) {
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

    long value = 0;
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

int read_long_long(long long* d) {
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

    long long value = 0;
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

int read_short(short* d) {
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
        *d = (short)(value * sign);
        return 1;
    }

    return 0;
}

int read_float(float* f) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Check for EOF
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

    // Read integer part
    float value = 0.0f;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c)) {
        int digit = c - '0';
        value = value * 10.0f + digit;
        read_any_digits = 1;
        c = getchar();
    }

    // Check for decimal point
    if (c == '.') {
        c = getchar();
        float divisor = 10.0f;

        // Read fractional part
        while (c != EOF && isdigit(c)) {
            int digit = c - '0';
            value += digit / divisor;
            divisor *= 10.0f;
            read_any_digits = 1;
            c = getchar();
        }
    }

    // Put back the last character
    if (c != EOF) {
        ungetc(c, stdin);
    }

    if (read_any_digits) {
        *f = value * sign;
        return 1;
    }

    return 0;
}

/*
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

int read_string(char* s, int max_chars) {
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    if (c == EOF) {
        return -1;
    }

    int count = 0;
    while (c != EOF && !isspace(c) && count < max_chars - 1) {
        s[count++] = (char)c;
        c = getchar();
    }

    s[count] = '\0';

    if (c != EOF) {
        ungetc(c, stdin);
    }

    return (count > 0) ? 1 : 0;
}

/*
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

// my scanf() function
int my_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int assigned_count = 0;
    int i = 0;

    while (format[i] != '\0') {
        if (format[i] == '%') {
            i++;  // Move past '%'
            if (format[i] == '\0') {
                break;
            }

            // Parse the format specification
            FormatSpecifier spec;
            int consumed = parse_format_specifier(&format[i], &spec);
            i += consumed;

            // Handle based on specifier (ignore modifiers for now)
            switch (spec.specifier) {
                case 'd': {
                    int result;

                    // Check for length modifiers
                    if (strcmp(spec.length_mod, "ll") == 0) {
                        if (spec.suppress) {
                            long long temp;
                            result = read_long_long(&temp);
                        } else {
                            long long *ptr = va_arg(args, long long*);
                            result = read_long_long(ptr);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else if (strcmp(spec.length_mod, "l") == 0) {
                        if (spec.suppress) {
                            long temp;
                            result = read_long(&temp);
                        } else {
                            long *ptr = va_arg(args, long*);
                            result = read_long(ptr);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else if (strcmp(spec.length_mod, "h") == 0) {
                        if (spec.suppress) {
                            short temp;
                            result = read_short(&temp);
                        } else {
                            short *ptr = va_arg(args, short*);
                            result = read_short(ptr);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else {
                        // Default: regular int
                        if (spec.suppress) {
                            int temp;
                            result = read_integer(&temp);
                        } else {
                            int *ptr = va_arg(args, int*);
                            result = read_integer(ptr);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    }

                    // Handle EOF/failure
                    if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    } else if (result == 0) {
                        va_end(args);
                        return assigned_count;
                    }
                    break;
                }

                case 'c': {
                    int result;

                    if (spec.suppress) {
                        char temp;
                        result = read_char(&temp);
                        // Don't increment assigned_count
                        // Don't call va_arg
                    } else {
                        // Normal case - get pointer from va_arg and store
                        char *ptr = va_arg(args, char*);
                        result = read_char(ptr);
                        if (result == 1) {
                            assigned_count++;
                        }
                    }

                    // Handle EOF/failure
                    if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    }
                    break;
                }

                case 's': {
                    char buffer[256];  // Temp buffer for suppressed reads
                    int max_size = spec.field_width > 0 ? spec.field_width + 1 : 256;
                    int result;

                    if (spec.suppress) {
                        // Read but don't store
                        result = read_string(buffer, max_size);
                        // Don't increment assigned_count
                        // Don't call va_arg
                    } else {
                        // Normal case - get pointer from va_arg and store
                        char *ptr = va_arg(args, char*);
                        result = read_string(ptr, max_size);
                        if (result == 1) {
                            assigned_count++;
                        }
                    }

                    // Handle EOF/failure
                    if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    } else if (result == 0) {
                        va_end(args);
                        return assigned_count;
                    }
                    break;
                }

                case 'f': {
                    int result;

                    if (spec.suppress) {
                        // Read but don't store
                        float temp;
                        result = read_float(&temp);
                    } else {
                        // Normal case - get pointer from va_arg and store
                        float *ptr = va_arg(args, float*);
                        result = read_float(ptr);
                        if (result == 1) {
                            assigned_count++;
                        }
                    }

                    // Handle EOF/failure
                    if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    } else if (result == 0) {
                        va_end(args);
                        return assigned_count;
                    }
                    break;
                }

                default:
                    // Unknown specifier - skip it
                    break;
            }

            continue;  // Skip the i++ at the end of the while loop
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

        i++;
    }

    va_end(args);
    return assigned_count;
}

/*
int main() {
    // testing bad pointer assignment to scanf()
    float res;
    my_scanf("%f", &res);
    printf("%f",res);
}
*/