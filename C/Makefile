# Variables
OUTPUT := build/main
CFLAGS := -Iinclude
SRC := src/main.c src/assembler.c src/dynamic_array.c src/table.c src/instruction.c src/register.c
ARG := assembly/test.asm

# Special targets
.DELETE_ON_ERROR:
.PHONY: all build run clean

all: clean build run

# Create executable
build: $(OUTPUT)

$(OUTPUT): $(SRC)
	@mkdir -p build
	@clang $(CFLAGS) $(SRC) -o $(OUTPUT)
	@echo "Produced program executable"

# Run Executable
run: build
	@echo "Running program executable"
	@./$(OUTPUT) $(ARG)

# Clean build directory
clean:
	@clear
	@rm -rf build
	@echo "Cleaned build directory"