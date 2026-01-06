# Define the compiler
CC = gcc

# Define flags (Wall shows all warnings, useful for debugging!)
CFLAGS = -Wall -Wextra -g

# The final executable names
TARGETS = test_my_scanf demo_program

# Source files
TEST_SRCS = test_my_scanf.c my_scanf.c
DEMO_SRCS = demo_program.c my_scanf.c

all: $(TARGETS)

test_my_scanf: $(TEST_SRCS)
	$(CC) $(CFLAGS) $(TEST_SRCS) -o test_my_scanf

demo_program: $(DEMO_SRCS)
	$(CC) $(CFLAGS) $(DEMO_SRCS) -o demo_program

test: test_my_scanf
	./test_my_scanf

demo: demo_program
	./demo_program

clean:
	rm -f $(TARGETS)
	rm -rf *.dSYM

.PHONY: all test demo clean