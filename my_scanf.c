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
#include <stdlib.h>
#include <string.h>

// parameters:
// - have a sequence that parses the input string to look for mulitple values. maybe regex?
int my_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int assinged_count = 0;

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            // temporary; have to figure out how to get numbers later
            // probably with a while loop
            char specifier = format[i + 1];

            switch(specifier) {
                case 'd': {
                    // might wrap the code for cases into helper functions to make
                    // code more readable; need to learn more about public/private
                    // access in c
                    int *ptr = va_arg(args, int*);

                    // Skip whitespace
                    int c = getchar();
                    while (isspace(c)) {
                        c = getchar();
                    }

                    // Check for sign
                    int sign = 1;
                    if (c == '-') {
                        sign = -1;
                        c = getchar();
                    } else if (c == '+') {
                        c = getchar();
                    }

                    // SRead digits and build the number
                    int value = 0;
                    int read_any_digits = 0;

                    while (isdigit(c)) {
                        int digit = c - '0';
                        value = value * 10 + digit;
                        read_any_digits = 1;
                        c = getchar();
                    }

                    // Only assign if we successfully read digits
                    if (read_any_digits) {
                        *ptr = value * sign;
                        assinged_count++;
                    } else {
                        printf("Failed to read integer!\n");
                    }

                    break;
                }
            }
        }
    }

    va_end(args);
    return assinged_count;
}

int main()
{
    // playing around with scanf to see how it works
    // char x[] = "";
    // scanf("%s", &x);
    // printf(x);
    int c;
    int f;
    printf("Enter an integer (for scanf): ");
    scanf("%d", &c);
    printf("Enter an integer (for my_scanf): ");
    my_scanf("%d", &f);
    printf("scanf: %d\n", c);
    printf("my_scanf: %d\n", f);
    return 0;
}