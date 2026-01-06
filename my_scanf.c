#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>


// FORMAT SPECIFIER STRUCTURE
// Represents a parsed format specifier like "%*5ld" or "%!3q"
// Components are parsed in order: %[*][!][width][length]specifier
typedef struct {
    char specifier;      // 'd', 's', 'c', 'f', 'x', 'z', 'q', 'b'
    int field_width;     // if app. for %31s, this is 31; 0 means no limit
    char length_mod[3];  // "ll", "l", "h", or ""
    int suppress;        // 1 if '*' is present, 0 otherwise
    int exclaim;         // 1 if '!' is present, 0 otherwise for %z and %q
} FormatSpecifier;

// FORMAT SPECIFIER PARSER
// Parses a format specifier starting immediately after '%'
// Format: %[*][!][width][length]specifier
// Examples:
//   "%5d"   → width=5, specifier='d'
//   "%*ld"  → suppress=1, length_mod="l", specifier='d'
//   "%!3q"  → exclaim=1, width=3, specifier='q'
// Returns: Number of characters consumed from the format string
static int parse_format_specifier(const char *format, FormatSpecifier *spec) {
    int pos = 0;

    // Initialize
    spec->field_width = 0;
    spec->length_mod[0] = '\0';
    spec->specifier = '\0';
    spec->suppress = 0;
    spec->exclaim = 0;  // Renamed from invert_case

    // Check for assignment suppression '*' first
    if (format[pos] == '*') {
        spec->suppress = 1;
        pos++;
    }

    // Check for exclamation '!'
    if (format[pos] == '!') {
        spec->exclaim = 1;
        pos++;
    }

    // Parse field width (digits) next
    while (isdigit(format[pos])) {
        spec->field_width = spec->field_width * 10 + (format[pos] - '0');
        pos++;
    }

    // Parse length modifier (h, l, ll, L)
    if (format[pos] == 'h') {
        spec->length_mod[0] = 'h';
        spec->length_mod[1] = '\0';
        pos++;
    } else if (format[pos] == 'L') {
        spec->length_mod[0] = 'L';
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

// HELPER FUNCTIONS to my_scanf()
// Whitespace Handling
static void skip_whitespace() {
    int c;
    while ((c = getchar()) != EOF && isspace(c)) {
        // Keep reading until non-whitespace
    }
    if (c != EOF) {
        ungetc(c, stdin);  // Put back the non-whitespace char
    }
}

// STANDARD TYPE READERS
// Each read_* function:
//   1. Skips leading whitespace
//   2. Checks for optional sign (+/-)
//   3. Reads digits/characters up to field_width limit
//   4. Puts back the first non-matching character with ungetc()
//   5. Returns: 1 (success), 0 (no valid input), -1 (EOF before any input)

// Read signed integer with optional field width limit
int read_integer(int* d, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Now c is either a non-whitespace char or EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    int value = 0;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c) && chars_read < max_chars) {
        int digit = c - '0';
        value = value * 10 + digit;
        read_any_digits = 1;
        chars_read++;
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

// Variations of the read_integer function for different sizes of integers
int read_long(long* d, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Now c is either a non-whitespace char or EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    long value = 0;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c) && chars_read < max_chars) {
        int digit = c - '0';
        value = value * 10 + digit;
        read_any_digits = 1;
        chars_read++;
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

int read_long_long(long long* d, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Now c is either a non-whitespace char or EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    long long value = 0;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c) && chars_read < max_chars) {
        int digit = c - '0';
        value = value * 10 + digit;
        read_any_digits = 1;
        chars_read++;
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

int read_short(short* d, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Now c is either a non-whitespace char or EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    int value = 0;
    int read_any_digits = 0;

    while (c != EOF && isdigit(c) && chars_read < max_chars) {
        int digit = c - '0';
        value = value * 10 + digit;
        read_any_digits = 1;
        chars_read++;
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

// Read float with decimal point and optional scientific notation (e/E)
int read_float(float *value, int field_width) {
    int c;
    char buffer[256] = {0};
    int pos = 0;
    int max_chars = (field_width > 0) ? field_width : 255;

    // Skip leading whitespace
    do {
        c = getchar();
    } while (isspace(c) && c != '\n');

    if (c == EOF) {
        return -1;
    }

    // Read sign
    if (c == '+' || c == '-') {
        buffer[pos++] = (char)c;
        c = getchar();
    }

    // Check if we have any digits
    int found_digit = 0;

    // Read digits before decimal point
    while (isdigit(c) && pos < max_chars) {
        buffer[pos++] = (char)c;
        found_digit = 1;
        c = getchar();
    }

    // Read decimal point and digits after
    if (c == '.' && pos < max_chars) {
        buffer[pos++] = (char)c;
        c = getchar();
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            found_digit = 1;
            c = getchar();
        }
    }

    // Handle scientific notation (e or E)
    if ((c == 'e' || c == 'E') && found_digit && pos < max_chars) {
        buffer[pos++] = (char)c;  // Add the 'e' or 'E'
        c = getchar();

        // Optional sign after e/E
        if ((c == '+' || c == '-') && pos < max_chars) {
            buffer[pos++] = (char)c;
            c = getchar();
        }

        // Must have at least one digit after e/E
        int exp_digit_count = 0;
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            exp_digit_count++;
            c = getchar();
        }

        // If no digits after e/E, the 'e' was not part of the number
        // Put back the last character and remove the 'e' from buffer
        if (exp_digit_count == 0) {
            if (c != EOF) ungetc(c, stdin);
            // Remove the 'e' and any sign that followed it
            pos = (int)strlen(buffer);
            while (pos > 0 && (buffer[pos-1] == 'e' || buffer[pos-1] == 'E' ||
                              buffer[pos-1] == '+' || buffer[pos-1] == '-')) {
                pos--;
            }
        }
    }

    // Put back the character we didn't use
    if (c != EOF) {
        ungetc(c, stdin);
    }

    // Null terminate and convert
    buffer[pos] = '\0';

    if (!found_digit) {
        return 0;  // No valid float
    }

    *value = strtof(buffer, NULL);
    return 1;
}

// Variations of the read_float function for different sizes of floats
int read_double(double *value, int field_width) {
    int c;
    char buffer[256] = {0};
    int pos = 0;
    int max_chars = (field_width > 0) ? field_width : 255;

    // Skip leading whitespace
    do {
        c = getchar();
    } while (isspace(c) && c != '\n');

    if (c == EOF) {
        return -1;
    }

    // Read sign
    if (c == '+' || c == '-') {
        buffer[pos++] = (char)c;
        c = getchar();
    }

    // Check if we have any digits
    int found_digit = 0;

    // Read digits before decimal point
    while (isdigit(c) && pos < max_chars) {
        buffer[pos++] = (char)c;
        found_digit = 1;
        c = getchar();
    }

    // Read decimal point and digits after
    if (c == '.' && pos < max_chars) {
        buffer[pos++] = (char)c;
        c = getchar();
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            found_digit = 1;
            c = getchar();
        }
    }

    // Handle scientific notation (e or E)
    if ((c == 'e' || c == 'E') && found_digit && pos < max_chars) {
        buffer[pos++] = (char)c;  // Add the 'e' or 'E'
        c = getchar();

        // Optional sign after e/E
        if ((c == '+' || c == '-') && pos < max_chars) {
            buffer[pos++] = (char)c;
            c = getchar();
        }

        // Must have at least one digit after e/E
        int exp_digit_count = 0;
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            exp_digit_count++;
            c = getchar();
        }

        // If no digits after e/E, the 'e' was not part of the number
        if (exp_digit_count == 0) {
            if (c != EOF) ungetc(c, stdin);
            pos = (int)strlen(buffer);
            while (pos > 0 && (buffer[pos-1] == 'e' || buffer[pos-1] == 'E' ||
                              buffer[pos-1] == '+' || buffer[pos-1] == '-')) {
                pos--;
            }
        }
    }

    // Put back the character we didn't use
    if (c != EOF) {
        ungetc(c, stdin);
    }

    // Null terminate and convert
    buffer[pos] = '\0';

    if (!found_digit) {
        return 0;  // No valid float
    }

    *value = strtod(buffer, NULL);
    return 1;
}

int read_long_double(long double *value, int field_width) {
    int c;
    char buffer[256] = {0};
    int pos = 0;
    int max_chars = (field_width > 0) ? field_width : 255;

    // Skip leading whitespace
    do {
        c = getchar();
    } while (isspace(c) && c != '\n');

    if (c == EOF) {
        return -1;
    }

    // Read sign
    if (c == '+' || c == '-') {
        buffer[pos++] = (char)c;
        c = getchar();
    }

    // Check if we have any digits
    int found_digit = 0;

    // Read digits before decimal point
    while (isdigit(c) && pos < max_chars) {
        buffer[pos++] = (char)c;
        found_digit = 1;
        c = getchar();
    }

    // Read decimal point and digits after
    if (c == '.' && pos < max_chars) {
        buffer[pos++] = (char)c;
        c = getchar();
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            found_digit = 1;
            c = getchar();
        }
    }

    // Handle scientific notation (e or E)
    if ((c == 'e' || c == 'E') && found_digit && pos < max_chars) {
        buffer[pos++] = (char)c;  // Add the 'e' or 'E'
        c = getchar();

        // Optional sign after e/E
        if ((c == '+' || c == '-') && pos < max_chars) {
            buffer[pos++] = (char)c;
            c = getchar();
        }

        // Must have at least one digit after e/E
        int exp_digit_count = 0;
        while (isdigit(c) && pos < max_chars) {
            buffer[pos++] = (char)c;
            exp_digit_count++;
            c = getchar();
        }

        // If no digits after e/E, the 'e' was not part of the number
        if (exp_digit_count == 0) {
            if (c != EOF) ungetc(c, stdin);
            pos = (int)strlen(buffer);
            while (pos > 0 && (buffer[pos-1] == 'e' || buffer[pos-1] == 'E' ||
                              buffer[pos-1] == '+' || buffer[pos-1] == '-')) {
                pos--;
            }
        }
    }

    // Put back the character we didn't use
    if (c != EOF) {
        ungetc(c, stdin);
    }

    // Null terminate and convert
    buffer[pos] = '\0';

    if (!found_digit) {
        return 0;  // No valid float
    }

    *value = strtold(buffer, NULL);
    return 1;
}

// Read hexadecimal integer (0-9, a-f, A-F) with optional 0x prefix
int read_hex_integer(int* x, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Check for EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    // Check for optional 0x or 0X prefix
    if (c == '0' && chars_read < max_chars) {
        chars_read++;
        int next = getchar();
        if ((next == 'x' || next == 'X') && chars_read < max_chars) {
            chars_read++;
            c = getchar();  // Move past the 'x' or 'X'
        } else {
            // Put back the character after '0'
            if (next != EOF) {
                ungetc(next, stdin);
            }
            // '0' is a valid hex digit, so c is already '0'
        }
    }

    int value = 0;
    int read_any_digits = 0;

    while (c != EOF && isxdigit(c) && chars_read < max_chars) {
        int digit;
        if (isdigit(c)) {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            break;
        }
        value = value * 16 + digit;
        read_any_digits = 1;
        chars_read++;
        c = getchar();
    }

    // Put back the last character
    if (c != EOF) {
        ungetc(c, stdin);
    }

    if (read_any_digits) {
        *x = value * sign;
        return 1;
    }

    return 0;
}

int read_char(char* c, int field_width) {
    int chars_to_read = (field_width > 0) ? field_width : 1;

    for (int i = 0; i < chars_to_read; i++) {
        int ch = getchar();
        if (ch == EOF) {
            return (i == 0) ? -1 : 1;  // EOF on first char is error, otherwise success
        }
        c[i] = (char)ch;
    }

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

// CUSTOM TYPE READERS
// These implement non-standard format specifiers as extensions to scanf

// %b - Binary integer reader (accepts 0s and 1s with optional 0b prefix)
int read_binary_integer(int* b, int field_width) {
    // Skip leading whitespace
    int c = getchar();
    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    // Check for EOF
    if (c == EOF) {
        return -1;
    }

    int chars_read = 0;
    int max_chars = (field_width > 0) ? field_width : INT_MAX;

    // Check for sign
    int sign = 1;
    if ((c == '-' || c == '+') && chars_read < max_chars) {
        if (c == '-') {
            sign = -1;
        }
        chars_read++;
        c = getchar();
    }

    // Check for optional 0b or 0B prefix
    if (c == '0' && chars_read < max_chars) {
        chars_read++;
        int next = getchar();
        if ((next == 'b' || next == 'B') && chars_read < max_chars) {
            chars_read++;
            c = getchar();  // Move past the 'b' or 'B'
        } else {
            // Put back the character after '0'
            if (next != EOF) {
                ungetc(next, stdin);
            }
            // '0' is a valid binary digit, so continue
        }
    }

    int value = 0;
    int read_any_digits = 0;

    while (c != EOF && (c == '0' || c == '1') && chars_read < max_chars) {
        int digit = c - '0';
        value = value * 2 + digit;
        read_any_digits = 1;
        chars_read++;
        c = getchar();
    }

    // Put back the last character
    if (c != EOF) {
        ungetc(c, stdin);
    }

    if (read_any_digits) {
        *b = value * sign;  // Apply sign to the magnitude
        return 1;
    }

    return 0;
}

// %q - Caesar cipher reader with rotation offset
// Reads text until newline, applies Caesar cipher with given offset
// Format: %Nq where N is rotation amount (e.g., %3q rotates by 3)
// %!Nq includes case inversion (uppercase ↔ lowercase after rotation)
// Non-letters (spaces, punctuation, numbers) pass through unchanged
int read_cipher(char* q, int offset, int invert_case, int max_size) {
    int c = getchar();

    // Read until newline or EOF
    int count = 0;

    while (c != EOF && c != '\n' && count < max_size - 1) {
        char result;

        if (c >= 'a' && c <= 'z') {
            // Lowercase letter - cipher it
            int pos = c - 'a';
            pos = (pos + offset) % 26;
            if (pos < 0) pos += 26;
            result = (char)('a' + pos);

            // Invert case if needed
            if (invert_case) {
                result = (char)(result - 'a' + 'A');  // Convert to uppercase
            }
        } else if (c >= 'A' && c <= 'Z') {
            // Uppercase letter - cipher it
            int pos = c - 'A';
            pos = (pos + offset) % 26;
            if (pos < 0) pos += 26;
            result = (char)('A' + pos);

            // Invert case if needed
            if (invert_case) {
                result = (char)(result - 'A' + 'a');  // Convert to lowercase
            }
        } else {
            // Non-letter (space, punctuation, numbers, etc.)
            result = (char)c;
        }

        q[count] = result;
        count++;
        c = getchar();
    }

    // Put back newline if we stopped at one
    if (c == '\n') {
        ungetc(c, stdin);
    }

    // Null-terminate
    q[count] = '\0';

    return (count > 0) ? 1 : -1;
}

// %z - "Gen Z" text reader that appends slang suffix
// Reads text until newline, trims trailing whitespace, adds suffix
// Variants:
//   %z   → appends " lol"
//   %!z  → appends " lol!"
//   %!hz → appends " haha!"
int read_gen_z(char* z, int max_size, int exclaim, const char *length_mod) {
    int c = getchar();

    // Skip leading whitespace
    while (c != EOF && isspace(c) && c != '\n') {
        c = getchar();
    }

    // Determine what to append
    const char *suffix;
    if (exclaim && length_mod[0] == 'h') {
        suffix = " haha!";  // %!hz
    } else if (exclaim) {
        suffix = " lol!";    // %!z
    } else {
        suffix = " lol";     // %z
    }

    int suffix_len = (int)strlen(suffix);

    // If we hit newline or EOF immediately, return just the suffix (without leading space)
    if (c == '\n' || c == EOF) {
        if (max_size >= suffix_len) {
            strcpy(z, suffix + 1);  // Skip leading space
            return 1;
        }
        return 0;
    }

    // Read until newline or EOF
    int count = 0;
    int last_non_space = -1;  // Track position of last non-whitespace char

    while (c != EOF && c != '\n' && count < max_size - suffix_len - 1) {
        z[count] = (char)c;
        if (!isspace(c)) {
            last_non_space = count;
        }
        count++;
        c = getchar();
    }

    // Put back newline if we stopped at one
    if (c == '\n') {
        ungetc(c, stdin);
    }

    // Trim trailing whitespace
    if (last_non_space >= 0) {
        count = last_non_space + 1;
    } else {
        // All whitespace - return just suffix (without leading space)
        strcpy(z, suffix + 1);
        return 1;
    }

    // Append suffix
    strcpy(z + count, suffix);

    return 1;
}

// MAIN SCANF IMPLEMENTATION
// Custom implementation of scanf() with additional format specifiers
// Standard specifiers: %d, %ld, %lld, %hd, %f, %lf, %Lf, %x, %s, %c
// Custom specifiers:   %b (binary), %z (gen-z text), %q (cipher)
// Modifiers:           * (suppress), ! (custom modifier), field width
// Format string processing:
//   - '%%' → matches literal '%' in input
//   - Whitespace in format → skips any amount of whitespace in input
//   - Literal characters → must match exactly in input
//   - Format specifiers → parsed and dispatched to appropriate reader
//
// Return value: Number of successfully assigned items (not suppressed)
//               Returns EOF (-1) if EOF encountered before any assignment
int my_scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int assigned_count = 0;
    int i = 0;

    // Main parsing loop - process format string character by character
    while (format[i] != '\0') {
        // Format specifier found - parse and process it
        if (format[i] == '%') {
            i++;
            if (format[i] == '\0') {
                break;
            }

            // Handle %% as a literal '%'
            if (format[i] == '%') {
                // Skip any whitespace before the % if needed
                int c = getchar();
                if (c != '%') {
                    // Mismatch - the literal % is missing
                    if (c != EOF) {
                        ungetc(c, stdin);
                    }
                    va_end(args);
                    return assigned_count;  // Return what we've assigned so far
                }
                // Successfully matched the literal %
                i++;  // Move past the second '%'
                continue;
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
                            result = read_long_long(&temp, spec.field_width);
                        } else {
                            long long *ptr = va_arg(args, long long*);
                            result = read_long_long(ptr, spec.field_width);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else if (strcmp(spec.length_mod, "l") == 0) {
                        if (spec.suppress) {
                            long temp;
                            result = read_long(&temp, spec.field_width);
                        } else {
                            long *ptr = va_arg(args, long*);
                            result = read_long(ptr, spec.field_width);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else if (strcmp(spec.length_mod, "h") == 0) {
                        if (spec.suppress) {
                            short temp;
                            result = read_short(&temp, spec.field_width);
                        } else {
                            short *ptr = va_arg(args, short*);
                            result = read_short(ptr, spec.field_width);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else {
                        // Default: regular int
                        if (spec.suppress) {
                            int temp;
                            result = read_integer(&temp, spec.field_width);
                        } else {
                            int *ptr = va_arg(args, int*);
                            result = read_integer(ptr, spec.field_width);
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
                    int num_chars = (spec.field_width > 0) ? spec.field_width : 1;

                    if (spec.suppress) {
                        // Read but don't store - use a temporary buffer
                        char temp[256];
                        result = read_char(temp, num_chars);
                    } else {
                        // Normal case - get pointer from va_arg and store
                        char *ptr = va_arg(args, char*);
                        result = read_char(ptr, num_chars);
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

                    // Check for length modifiers
                    if (strcmp(spec.length_mod, "L") == 0) {
                        if (spec.suppress) {
                            long double temp;
                            result = read_long_double(&temp, spec.field_width);
                        } else {
                            long double *ptr = va_arg(args, long double*);
                            result = read_long_double(ptr, spec.field_width);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else if (strcmp(spec.length_mod, "l") == 0) {
                        if (spec.suppress) {
                            double temp;
                            result = read_double(&temp, spec.field_width);
                        } else {
                            double *ptr = va_arg(args, double*);
                            result = read_double(ptr, spec.field_width);
                            if (result == 1) {
                                assigned_count++;
                            }
                        }
                    } else {
                        // Default: regular float
                        if (spec.suppress) {
                            float temp;
                            result = read_float(&temp, spec.field_width);
                        } else {
                            float *ptr = va_arg(args, float*);
                            result = read_float(ptr, spec.field_width);
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

                case 'x': {
                    int result;

                    if (spec.suppress) {
                        // Read but don't store
                        int temp;
                        result = read_hex_integer(&temp, spec.field_width);
                    } else {
                        // Normal case - get pointer from va_arg and store
                        int *ptr = va_arg(args, int*);
                        result = read_hex_integer(ptr, spec.field_width);
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

                case 'b': {
                    int result;

                    if (spec.suppress) {
                        // Read but don't store
                        int temp;
                        result = read_binary_integer(&temp, spec.field_width);
                    } else {
                        // Normal case - get pointer from va_arg and store
                        int *ptr = va_arg(args, int*);
                        result = read_binary_integer(ptr, spec.field_width);
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

                case 'z': {
                    char buffer[256];  // Temp buffer for suppressed reads
                    int max_size = spec.field_width > 0 ? spec.field_width : 256;
                    int result;

                    if (spec.suppress) {
                        result = read_gen_z(buffer, max_size, spec.exclaim, spec.length_mod);
                    } else {
                        char *ptr = va_arg(args, char*);
                        result = read_gen_z(ptr, max_size, spec.exclaim, spec.length_mod);
                        if (result == 1) {
                            assigned_count++;
                        }
                    }

                    // Handle EOF - will handle whitespaces so don't worry
                    if (result == 0) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    }
                    break;
                }

                case 'q': {
                    char buffer[256];  // Temp buffer for suppressed reads
                    int max_size = 256;
                    int offset = spec.field_width;
                    int result;

                    if (spec.suppress) {
                        result = read_cipher(buffer, offset, spec.exclaim, max_size);
                    } else {
                        char *ptr = va_arg(args, char*);
                        result = read_cipher(ptr, offset, spec.exclaim, max_size);
                        if (result == 1) {
                            assigned_count++;
                        }
                    }

                    // Handle EOF
                    if (result == -1) {
                        va_end(args);
                        return (assigned_count == 0) ? -1 : assigned_count;
                    }
                    break;
                }

                default:
                    // Unknown specifier - skip it
                    break;
            }

            continue;
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
