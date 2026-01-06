#include "my_scanf.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEMO_BUFFER_SIZE 256
#define MAX_ARGS 10

// Format specifier parser matching my_scanf's implementation
typedef struct {
    char type;
    int field_width;
    char length_mod[3];
    int suppress;
    int exclaim;
} FormatSpec;

int parse_format(const char *format, FormatSpec specs[], int max_specs) {
    int count = 0;
    const char *p = format;

    while (*p && count < max_specs) {
        if (*p == '%') {
            p++;
            if (*p == '%') {
                p++;
                continue;
            }

            specs[count].suppress = 0;
            specs[count].exclaim = 0;
            specs[count].field_width = 0;
            specs[count].length_mod[0] = '\0';

            if (*p == '*') {
                specs[count].suppress = 1;
                p++;
            }

            if (*p == '!') {
                specs[count].exclaim = 1;
                p++;
            }

            while (isdigit(*p)) {
                specs[count].field_width = specs[count].field_width * 10 + (*p - '0');
                p++;
            }

            if (*p == 'h') {
                specs[count].length_mod[0] = 'h';
                specs[count].length_mod[1] = '\0';
                p++;
            } else if (*p == 'L') {
                specs[count].length_mod[0] = 'L';
                specs[count].length_mod[1] = '\0';
                p++;
            } else if (*p == 'l') {
                specs[count].length_mod[0] = 'l';
                if (*(p + 1) == 'l') {
                    specs[count].length_mod[1] = 'l';
                    specs[count].length_mod[2] = '\0';
                    p += 2;
                } else {
                    specs[count].length_mod[1] = '\0';
                    p++;
                }
            }

            if (*p) {
                specs[count].type = *p;
                count++;
                p++;
            }
        } else {
            p++;
        }
    }

    return count;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char format_str[DEMO_BUFFER_SIZE];
    char input_line[DEMO_BUFFER_SIZE];

    printf("=== my_scanf Interactive Demo ===\n");
    printf("Enter 'quit' or '-1' at any prompt to exit\n\n");

    while (1) {
        printf("Enter a format string for my_scanf: ");

        if (fgets(format_str, sizeof(format_str), stdin) == NULL) {
            printf("\nEOF encountered. Exiting.\n");
            break;
        }

        size_t len = strlen(format_str);
        if (len > 0 && format_str[len - 1] == '\n') {
            format_str[len - 1] = '\0';
        }

        if (strcmp(format_str, "quit") == 0 || strcmp(format_str, "-1") == 0) {
            printf("Exiting demo.\n");
            break;
        }

        if (strlen(format_str) == 0) {
            printf("Empty format string. Try again.\n\n");
            continue;
        }

        FormatSpec specs[MAX_ARGS];
        int num_specs = parse_format(format_str, specs, MAX_ARGS);

        printf("my_scanf(%s): ", format_str);

        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            printf("\nEOF encountered. Exiting.\n");
            break;
        }

        len = strlen(input_line);
        if (len > 0 && input_line[len - 1] == '\n') {
            input_line[len - 1] = '\0';
        }

        if (strcmp(input_line, "quit") == 0 || strcmp(input_line, "-1") == 0) {
            printf("Exiting demo.\n");
            break;
        }

        FILE *temp = tmpfile();
        if (temp == NULL) {
            fprintf(stderr, "Error creating temporary file\n");
            continue;
        }

        fprintf(temp, "%s\n", input_line);
        rewind(temp);

        FILE *original_stdin = stdin;
        stdin = temp;

        // Allocate storage
        void *args[MAX_ARGS];
        int int_vals[MAX_ARGS] = {0};
        long long_vals[MAX_ARGS] = {0};
        long long longlong_vals[MAX_ARGS] = {0};
        short short_vals[MAX_ARGS] = {0};
        float float_vals[MAX_ARGS] = {0};
        double double_vals[MAX_ARGS] = {0};
        long double ldouble_vals[MAX_ARGS] = {0};
        char string_vals[MAX_ARGS][DEMO_BUFFER_SIZE] = {{0}};
        char char_vals[MAX_ARGS] = {0};

        int arg_idx = 0;
        for (int i = 0; i < num_specs; i++) {
            if (specs[i].suppress) {
                continue;
            }

            switch (specs[i].type) {
                case 'd':
                case 'i':
                    if (strcmp(specs[i].length_mod, "ll") == 0) {
                        args[arg_idx++] = &longlong_vals[i];
                    } else if (strcmp(specs[i].length_mod, "l") == 0) {
                        args[arg_idx++] = &long_vals[i];
                    } else if (strcmp(specs[i].length_mod, "h") == 0) {
                        args[arg_idx++] = &short_vals[i];
                    } else {
                        args[arg_idx++] = &int_vals[i];
                    }
                    break;

                case 'f':
                    if (strcmp(specs[i].length_mod, "L") == 0) {
                        args[arg_idx++] = &ldouble_vals[i];
                    } else if (strcmp(specs[i].length_mod, "l") == 0) {
                        args[arg_idx++] = &double_vals[i];
                    } else {
                        args[arg_idx++] = &float_vals[i];
                    }
                    break;

                case 's':
                case 'z':
                case 'q':
                    args[arg_idx++] = string_vals[i];
                    break;

                case 'c':
                    args[arg_idx++] = &char_vals[i];
                    break;

                case 'x':
                case 'b':
                    args[arg_idx++] = &int_vals[i];
                    break;

                default:
                    args[arg_idx++] = &int_vals[i];
                    break;
            }
        }

        int result = 0;
        switch (arg_idx) {
            case 0: result = my_scanf(format_str); break;
            case 1: result = my_scanf(format_str, args[0]); break;
            case 2: result = my_scanf(format_str, args[0], args[1]); break;
            case 3: result = my_scanf(format_str, args[0], args[1], args[2]); break;
            case 4: result = my_scanf(format_str, args[0], args[1], args[2], args[3]); break;
            case 5: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4]); break;
            case 6: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4], args[5]); break;
            case 7: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break;
            case 8: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break;
            case 9: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]); break;
            case 10: result = my_scanf(format_str, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]); break;
        }

        stdin = original_stdin;
        fclose(temp);

        printf("---\n");
        printf("Fields matched: %d\n", result);

        if (result > 0) {
            int assigned_idx = 0;
            for (int i = 0; i < num_specs; i++) {
                char mod_str[16] = "";
                if (specs[i].suppress) strcat(mod_str, "*");
                if (specs[i].exclaim) strcat(mod_str, "!");
                if (specs[i].field_width > 0) {
                    char width_buf[8];
                    sprintf(width_buf, "%d", specs[i].field_width);
                    strcat(mod_str, width_buf);
                }
                if (specs[i].length_mod[0]) strcat(mod_str, specs[i].length_mod);

                if (specs[i].suppress) {
                    printf("  Field %d (%%%s%c): (suppressed)\n", i+1, mod_str, specs[i].type);
                    continue;
                }

                if (assigned_idx >= result) break;

                switch (specs[i].type) {
                    case 'd':
                    case 'i':
                        if (strcmp(specs[i].length_mod, "ll") == 0) {
                            printf("  Field %d (%%%s%c): %lld\n", i+1, mod_str, specs[i].type, longlong_vals[i]);
                        } else if (strcmp(specs[i].length_mod, "l") == 0) {
                            printf("  Field %d (%%%s%c): %ld\n", i+1, mod_str, specs[i].type, long_vals[i]);
                        } else if (strcmp(specs[i].length_mod, "h") == 0) {
                            printf("  Field %d (%%%s%c): %hd\n", i+1, mod_str, specs[i].type, short_vals[i]);
                        } else {
                            printf("  Field %d (%%%s%c): %d\n", i+1, mod_str, specs[i].type, int_vals[i]);
                        }
                        break;

                    case 'f':
                        if (strcmp(specs[i].length_mod, "L") == 0) {
                            printf("  Field %d (%%%s%c): %Lf\n", i+1, mod_str, specs[i].type, ldouble_vals[i]);
                        } else if (strcmp(specs[i].length_mod, "l") == 0) {
                            printf("  Field %d (%%%s%c): %lf\n", i+1, mod_str, specs[i].type, double_vals[i]);
                        } else {
                            printf("  Field %d (%%%s%c): %f\n", i+1, mod_str, specs[i].type, float_vals[i]);
                        }
                        break;

                    case 's':
                    case 'z':
                    case 'q':
                        printf("  Field %d (%%%s%c): %s\n", i+1, mod_str, specs[i].type, string_vals[i]);
                        break;

                    case 'c':
                        printf("  Field %d (%%%s%c): '%c'\n", i+1, mod_str, specs[i].type, char_vals[i]);
                        break;

                    case 'x':
                        printf("  Field %d (%%%s%c): 0x%x (%d)\n", i+1, mod_str, specs[i].type, int_vals[i], int_vals[i]);
                        break;

                    case 'b':
                        printf("  Field %d (%%%s%c): %d (binary)\n", i+1, mod_str, specs[i].type, int_vals[i]);
                        break;
                }

                assigned_idx++;
            }
        } else if (result == 0) {
            printf("  No valid input matched\n");
        } else if (result == -1) {
            printf("  EOF during parsing\n");
        }

        printf("\n");
    }

    return 0;
}