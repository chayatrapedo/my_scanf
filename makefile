# Define the compiler
CC = gcc

# Define flags (Wall shows all warnings, useful for debugging!)
CFLAGS = -Wall -Wextra -g

# The final executable name
TARGET = test_my_scanf

# The source files
SRCS = test_my_scanf.c my_scanf.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)