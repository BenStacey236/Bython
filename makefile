# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
OBJDIR = build

# Automatically find all C source files
SRCS := $(wildcard *.c)

# Generate object files in build/ directory
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

# Output binary
TARGET = tokeniser

# Default rule
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file to .o in build/
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)