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
 * Warning from CLion potentially about underlying operations:
 * Clang-Tidy: 'scanf' used to convert a string to an integer value,
 * but function will not report conversion errors; consider using
 * 'strtol' instead
 */

#include <stdio.h>
int main()
{
    // playing around with scanf to see how it works
    char x[] = "";
    scanf("%s", &x);
    printf(x);

    return 0;
}