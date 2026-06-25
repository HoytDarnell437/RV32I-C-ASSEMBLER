# Variables
OUTPUT := build/main
CFLAGS := -Iinclude
SRC := src/main.c src/assembler.c src/dynamic_array.c src/instruction.c
ARG := assembly/code.asm

# Special targets
.DELETE_ON_ERROR:
.PHONY: build run clean

# 1. Create executable
build: $(OUTPUT)

$(OUTPUT): $(SRC)
	@mkdir -p build
	@clang $(CFLAGS) $(SRC) -o $(OUTPUT)
	@echo "Produced program executable"

# 2. Run Executable
run: build
	@echo "Running program executable"
	@./$(OUTPUT) $(ARG)

# 3. Clean build directory
clean:
	@clear
	@rm -rf build
	@echo "Cleaned build directory"