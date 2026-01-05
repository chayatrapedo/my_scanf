# my_scanf
*COMP 2113 Final Project - Fall 2025*

This project implements a custom version of the C standard library function `scanf()`, called `my_scanf()`. The implementation recreates core `scanf` functionality **from scratch**, using only low-level input primitives, while closely matching standard `scanf` behavior.

Constraints satisfied:
- Written entirely in C
- Uses only `getc()` / `getchar()` for input
- Does **not** call `scanf()` internally

## Supported Format Specifiers

`my_scanf()` supports the following subset of `scanf`, including standard modifiers:

- `%d`: decimal integers (supports `h`, `l`, `ll` length modifiers)
- `%x`: hexadecimal integers
- `%f`: floating-point numbers (supports `l` for double and `L` for long double)
- `%c`: characters
- `%s`: strings

Behavior matches `scanf()` for return values, whitespace handling, partial matches, and failure cases.


## Custom Extensions / Modifiers

This project implements three custom format modifiers that utilize the field width and the unique `!` flag.

### 1. `%b` Binary Modifier
Reads a sequence of binary digits and converts them into an integer. It optionally recognizes `0b` or `0B` prefixes.

| Format | Input | Result (Stored Integer) |
| :--- | :--- | :--- |
| `%b` | `1010` | `10` |
| `%b` | `0b111` | `7` |
| `%b` | `-0b11` | `-3` |

### 2. `%z` Gen-Z Modifier
A string-processing modifier that appends "slang" suffixes to the input text. It trims trailing whitespace from input before appending the suffix. It also recognizes that Gen-Z uses filler words, and handles whitespace accordingly to their speech patterns.

| Format | Input | Suffix Applied | Final String Result |
| :--- | :--- | :--- | :--- |
| `%z` | `hello` | ` lol` | `"hello lol"` |
| `%!z` | `no cap` | ` lol!` | `"no cap lol!"` |
| `%!hz` | `bet` | ` haha!` | `"bet haha!"` |
| `%hz` | (whitespace) | ` haha` | `"haha"` |

### 3. `%q` Cipher Modifier
Implements a Caesar Cipher substitution. Unlike standard specifiers, `%q` uses the field width as the shift offset.

* **Offset**: Determined by the field width (e.g., `%3q` shifts 3 places).
* **Case Inversion (`!`)**: If present, the program shifts the letter and inverts its case.
* **Constraint**: Only shifts letters ($a-z$, $A-Z$); other characters are unchanged.

| Format | Input | Offset | Case Inversion | Result |
| :--- | :--- | :--- | :--- | :--- |
| `%1q` | `abc` | 1 | No | `bcd` |
| `%3q` | `XYZ` | 3 | No | `ABC` |
| `%!1q` | `abc` | 1 | Yes | `BCD` |
| `%!3q` | `ABC` | 3 | Yes | `def` |

## Build & Run

```bash
make
./test_my_scanf
```
Tests compare `my_scanf()` directly against the system `scanf()` for built-in `scanf()` modifiers. For custom modifiers (`%b`, `%z`, and `%q`), the tests verify the results against hardcoded expected values and manual calculations, as these specifiers are unique to this implementation.

## Testing
The project includes extensive unit tests covering:

-   **Valid and invalid input:** Ensures the function stops processing on a mismatch.
-   **Edge cases:** Handling of `INT_MAX`, `INT_MIN`, etc.
-   **EOF Handling:** Correctly returning `EOF` when the end of a stream is reached before any successful assignments.
-   **Partial matches and whitespace handling:** Mimics standard `scanf` by skipping leading whitespace for most specifiers and handling literal character matches in the format string.