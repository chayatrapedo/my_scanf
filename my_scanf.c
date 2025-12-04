/* Project Planning // Notes
 - the %d, %f, %x, %c, and %s options, with all of their modifiers

 * %f -> input floats
 * Input: 4.00000000001 --> output 4.000000 (only 6 decimal places)
 *
 * %d -> input integers (truncates input, if I input 4.5 it takes 4)
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
    float f;
    scanf("%f", &f);
    printf("%f", f);

    return 0;
}