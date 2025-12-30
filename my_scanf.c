/* Project Planning // Notes
 * - the %d, %f, %x, %c, and %s options, with all of their modifiers
 *
 * %f -> input floats
 * Input: 4.00000000001 --> output 4.000000 (only 6 decimal places)
 *
 * %d -> input integers (truncates input, if I input 4.5 it takes 4)
 *
 * %x - does hexadecimal --> does the same if I type 0x or nothing
 * abc = 2748
 * considers spaces/nonvalid characters as a break --> a b c is 10
 *
 * %c - gets a singular character
 *
 * %s - gets more than 1 character
 * i: "hey!" my name is chaya
 * o: "hey!"
 * -> seems to ignore punctuation, space
 * i: ... ...
 * o: ...
 * another:
 * hello, there friend "".
 * hello,
 * more:
 * i: "Hey,,,""",""",""
 * o: nothing
 * i: "hey" hey
 * o: "hey"
 *
 *
 * Warning from CLion potentially about underlying operations:
 * Clang-Tidy: 'scanf' used to convert a string to an integer value,
 * but function will not report conversion errors; consider using
 * 'strtol' instead
 *
 * Notes 12/07:
 * - Will have to be able to pass scanf a variable to return the input(s) too and make sure they
 *   match based on the flags
 * Personal functions for scanf:
 * '%z'    - gen z , add 'lol'
 * '%?', n - cipher, offset (is this doable?)
 *
 *
 * Notes on 12/08:
 * - Use regex
 * - How do you use a variable number of arguments? --> variadic functions in c --> stdarg.h
 * - get character - fopen to use the stream? getc from the stream? look into how it works
 * - %31f can be implemented (since it's a part of %f which also is a project requirement to implement)
 *
 * Notes on 12/15:
 * - %s has to alocate one more spot of memory to have the sentinal value (0)
 */

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
    skip_whitespace();

    int c = getchar();

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

    while (isdigit(c)) {
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

int read_char(char* c) {
    int ch = getchar();

    if (ch == EOF) {
        return 0;
    }

    *c = (char)ch;
    return 1;
}

int read_string(char* s) {
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
                /*
                case 'f': {
                    float *ptr = va_arg(args, float*);
                    if (read_float(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                case 'x': {
                    int *ptr = va_arg(args, int*);
                    if (read_hex_integer(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                case 'c': {
                    char *ptr = va_arg(args, char*);
                    if (read_char(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                case 's': {
                    char *ptr = va_arg(args, char*);
                    if (read_string(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                // ============================================
                // CUSTOM EXTENSIONS
                // ============================================

                case 'b': {
                    // Custom: Binary integer
                    int *ptr = va_arg(args, int*);
                    if (read_unsigned_binary_int(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                case 'q': {
                    // Custom: Cipher with offset (offset is next arg)
                    char *ptr = va_arg(args, char*);
                    int offset = va_arg(args, int);
                    if (read_cipher(ptr, offset)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }

                case 'z': {
                    // Custom: Gen Z string (adds "lol")
                    char *ptr = va_arg(args, char*);
                    if (read_gen_z(ptr)) {
                        assigned_count++;
                    } else {
                        va_end(args);
                        return assigned_count;
                    }
                    i++;
                    break;
                }
                */

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

/*
int main()
{
    // playing around with scanf to see how it works
    // char x[] = "";
    // scanf("%s", &x);
    // printf(x);
    int d = 0;
    int f = 0;
    printf("Enter an integer (for scanf): ");
    // if (scanf("%d", &d) != 1) {
    //     printf("Invalid input!\n");
    //     return 1;
    // }
    // printf("Enter an integer (for my_scanf): ");
    my_scanf("%d", &f);
    // printf("scanf: %d\n", d);
    printf("my_scanf: %d\n", f);
    return 0;
}
*/