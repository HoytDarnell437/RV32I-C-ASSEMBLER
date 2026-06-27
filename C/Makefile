# Variables
OUTPUT := build/main
CFLAGS := -Iinclude
#RUNFLAGS := ASAN_OPTIONS=detect_leaks=1
SRC := src/main.c src/assembler.c src/dynamic_array.c src/table.c src/instruction.c src/register.c
ARG := assembly/code.asm

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

leak: build
	@echo "Checking for memory leaks"
	@leaks --atExit -- ./$(OUTPUT) $(ARG)