/**
 * @file assembler.c
 * @brief Implementation for assembler.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/assembler.h"
#include "../include/dynamic_array.h"
#include "../include/parse_value.h"
#include "../include/instruction.h"
#include "../include/register.h"
#include "../include/table.h"

#define MAX_LINE_LENGTH 512

/**
 * @struct asm_t
 * @brief Struct holding the context for the assembler.
 */
typedef struct {
    const char *filename;               /**< Name of .asm file. */
    FILE *file;                         /**< Pointer to the .asm file. */
    array_t assembly;              /**< Array of the .asm file's lines. */
    array_t clean_assembly; /**< 2D Master array of the .asm file's tokens with comments removed. */
    table_t data_table;                 /**< Table with the data labels as keys and their addresses as corresponding values. */
    table_t text_table;                 /**< Table with the text labels as keys and their addresses as corresponding values. */
    table_t const_table;                /**< Table with constant labels as the keys and their corresponding values. */
    array_t data_image;             /**< Array of data initialization contents. */
    array_t instructions;   /**< Array of instructions. */
} asm_t;

// asm_t Associated Functions

/**
 * @brief Initializes all members of ctx.
 * @param ctx Pointer to the active assembler context structure.
 * @param[in] filename Name of the .asm file.
 */
static void asm_init(asm_t *ctx, const char *filename);

/**
 * @brief Frees all members of ctx.
 * @param ctx Pointer to the active assembler context structure.
 */
static void asm_free(asm_t *ctx);

/**
 * @brief Dumps all members of ctx to build/asm_dump.txt.
 * @param ctx Pointer to the active assembler context structure.
 */
static void asm_dump(asm_t *ctx);

/**
 * @brief Closes the .asm file and produces an error message.
 * @param ctx Pointer to the active assembler context structure.
 * @param[in] message Error message to output in terminal.
 */
static void asm_error(asm_t *ctx, const char *message);

// Assembly Parsing Pipeline

/**
 * @brief Reads the .asm file provided and store it in a dynamic array of type array_t.
 * @param ctx Pointer to the active assembler context structure.
 */
static void read_assembly(asm_t *ctx);

/**
 * @brief Tokenizes raw assembly lines into a structured 2D array.
 * Separates lines into separate tokens, removes comments, separates same line labels, and forces lowercase letters.
 * @param ctx Pointer to the active assembler context structure.
 */
static void format_assembly(asm_t *ctx);

/**
 * @brief Takes in the structured assembly and produces a constant value table, a table of instruction labels and their addresses,
 * a table of data labels and their addresses, an array of bytes making up the initial data image, and a list containing the instructions.
 *  @param ctx Pointer to the active assembler context structure.
 */
static void subroutine_gen(asm_t *ctx);

// Output Generators

/**
 * @brief Uses the data image produces in subroutine_gen to write the contents to a file named data.hex.
 * @param ctx Pointer to the active assembler context structure.
 */
static void create_data_file(asm_t *ctx);

/**
 * @brief Uses the array of instructions to create a file named instructions.hex.
 * @param ctx pointer to the active assembler context structure.
 */
static void create_instruction_file(asm_t *ctx);

// Helper Functions

/**
 * @brief Copy a string starting at one index and ending on another (inclusive).
 * @param str String to copy from.
 * @param start Index to start on.
 * @param end Index to end on (inclusive).
 * @param dest String to copy to.
 */
static void get_substring(const char *str, int start, int end, char *dest);

/**
 * @brief Duplicates an integer value to the heap and returns the pointer.
 * @param num Number to be duplicated.
 * @return Returns a pointer to the block allocated on the heap.
 */
static void *intdup(int num);

/**
 * @brief qsort comparison function for pointers
 */
int compare_pointer_addresses(const void *a, const void *b);

void assemble(const char *filename) {
    asm_t ctx;
    asm_init(&ctx, filename);

    read_assembly(&ctx);
    format_assembly(&ctx);
    subroutine_gen(&ctx);

    create_data_file(&ctx);
    create_instruction_file(&ctx);
    
    asm_dump(&ctx);
    asm_free(&ctx);
}

static void read_assembly(asm_t *ctx) {
    ctx->file = fopen(ctx->filename, "r");
    char *line = malloc(MAX_LINE_LENGTH);

    if (!ctx->file) {
        asm_error(ctx, "Error: Could not open .asm file");
    } else if (!line) {
        asm_error(ctx, "Error: Failed to allocate memory in read_assembly");
    }

    while (fgets(line, MAX_LINE_LENGTH, ctx->file) != NULL) {
        size_t len = strlen(line);

        if (len == MAX_LINE_LENGTH - 1 && line[len - 1] != '\n' && !feof(ctx->file)) {
            asm_error(ctx, "Error: Line in assembly file exceeds MAX_LINE_LENGTH");
        }

        line[strcspn(line, "\r\n")] = '\0';
        array_append(ctx->assembly, line);
        line = malloc(MAX_LINE_LENGTH);

        if (!line) {
            asm_error(ctx, "Error: Failed to allocate memory in read_assembly");
        }
    }
    
    free(line);

    fclose(ctx->file);
}

static void format_assembly(asm_t *ctx) {
    for (int i = 0; i < array_get_size(ctx->assembly); i++) {
        char *str = strdup(array_get(ctx->assembly, i));
        char *tok = strtok(str, " ,()");
        array_t sub_array = array_create(1);

        while (tok != NULL) {
            if (tok[0] == '#') {
                break;
            }

            for (int j = 0; tok[j] != '\0'; j++) {
                if (tok[j] == '\'') {
                    break;
                }
                if (tok[j] < 91 && tok[j] > 64) {
                    tok[j] += 32;
                }
            }
            
            if (strcmp(tok, "'") == 0) {
                array_append(sub_array, strdup("' '"));
                tok = strtok(NULL, " ,()");
            } else {
                array_append(sub_array, strdup(tok));
            }

            if (strchr(tok, ':') != NULL && strstr(array_get(ctx->assembly, i), ".equ") == NULL) {
                array_append(ctx->clean_assembly, array_dupe(sub_array));
                array_pop(sub_array);
            }

            tok = strtok(NULL, " ,()");
        }

        if (array_get_size(sub_array) != 0) {
            array_append(ctx->clean_assembly, sub_array);
        } else {
            array_free(sub_array);
        }

        free(str);
    }
}

static void subroutine_gen(asm_t *ctx) {
    const char *directive = ".text";
    int text_counter = 0;
    int data_counter = 0;

    for (int i = 0; i < array_get_size(ctx->clean_assembly); i++) {
        array_t line = (array_t) array_get(ctx->clean_assembly, i);
        const char *string = array_get(line, 0);
        if (strcmp(string, ".text") == 0) {
            directive = ".text";
        } else if (strcmp(string, ".data") == 0) {
            directive = ".data";
        } else if (strcmp(string, ".word") == 0) {
            for (int j = 1; j < array_get_size(line); j++) {
                const char *tok = array_get(line, j);
                int value = parse_value(tok);

                if (array_get_size(ctx->data_image) & 0b11) {
                    int count = array_get_size(ctx->data_image) & 0b11;

                    do {
                        array_append(ctx->data_image, intdup(0));
                        data_counter += 1;
                    } while (++count < 4);

                    const char *label = array_get((array_t) array_get(ctx->clean_assembly, i - 1), 0);

                    if (table_get(ctx->data_table, label, NULL) == 1) {
                        table_set(ctx->data_table, label, data_counter);
                    }
                }

                array_append(ctx->data_image, intdup(value >> 24 & 0x000000ff));
                array_append(ctx->data_image, intdup(value >> 16 & 0x000000ff));
                array_append(ctx->data_image, intdup(value >> 8 & 0x000000ff));
                array_append(ctx->data_image, intdup(value & 0x000000ff));
                data_counter += 4;
            }

        } else if (strcmp(string, ".byte") == 0) {
            for (int j = 1; j < array_get_size(line); j++) {
                const char *tok = array_get(line, j);
                int value = 0;
                value += parse_value(tok);
                array_append(ctx->data_image, intdup(value));
                data_counter += 1;
            }

        } else if (strcmp(string, ".space") == 0) {
            int space = parse_value(array_get(line, 1));

            for (int j = 0; j < space; j++) {
                array_append(ctx->data_image, intdup(0));
            }

            data_counter += space;

        } else if (strcmp(string, ".align") == 0) {
            int align_to = 1 << parse_value(array_get(line, 1));

            if (align_to >= 2) {
                int remainder = data_counter % align_to;

                if (remainder > 0) {

                    int space = align_to - remainder;

                    for (int j = 0; j < space; j++) {
                        array_append(ctx->data_image, intdup(0));
                    }

                    data_counter += space;
                }
            }
        } else if (array_get_size(line) > 1 && strcmp(array_get(line, 1), ".equ") == 0) {
            const char *label = string;
            char clean_label[32];
            get_substring(label, 0, strlen(label) - 2, clean_label);
            table_set(ctx->const_table, clean_label, parse_value(array_get(line, 2)));

        } else if (string[0] == '.') {

            asm_error(ctx, "Error: Unsupported riscv directive used\n");

        } else if (string[strlen(string) - 1] == ':') {
            char key[32];
            get_substring(string, 0, strlen(string) - 2, key);

            if (strcmp(directive, ".text") == 0) {
                table_set(ctx->text_table, key, text_counter);
            } else {
                table_set(ctx->data_table, key, data_counter);
            }
        } else {
            if (instruction_lookup(array_get(line, 0))) {
                text_counter += 4;
                array_append(ctx->instructions, line);
            } else if (psuedo_instruction_lookup(array_get(line, 0))) {
                append_psuedo_instruction(line, ctx->instructions, &text_counter);
            } else {
                fprintf(stderr, "Unsupported instruction used '%s'\n", (char *) array_get(line, 0));
                asm_error(ctx, "Unsupported instruction used\n");
            }
        }
    }
}

static void create_data_file(asm_t *ctx) {
    FILE *file = fopen("build/data.hex", "w");
    int line_count = array_get_size(ctx->data_image);

    if (!file) {
        asm_error(ctx, "Error: could not open build/data.hex\n");
    }

    if (line_count < 1) {
        fclose(file);
    } else {
        for (int i = 0; i < line_count; i++) {
            fprintf(file, "%02x", *(int *) array_get(ctx->data_image, i));

            if (((i + 1) & 0b11) == 0) {
                fputs("\n", file);
            }
        }
    }

    fclose(file);
}

static void create_instruction_file(asm_t *ctx) {
    FILE *file = fopen("build/instructions.hex", "w");

    if (!file) {
        asm_error(ctx, "Error: could not open build/instructions.hex\n");
    }

    if (array_get_size(ctx->instructions) < 1) {
        fclose(file);
        asm_error(ctx, "Error: Assembly given to assembler must have atleast one instruction\n");

    } else {
        int length = array_get_size(ctx->instructions);
        int pc = -4;
        int i = 0;

        do {
            const instruction_t *instruction;
            array_t line;
            const char *mnemonic;

            line = (array_t) array_get(ctx->instructions, i);
            int line_length = array_get_size(line);
            mnemonic = array_get(line, 0);
            instruction = instruction_lookup(mnemonic);
            pc += 4;

            int j = 0;
            int register1 = -1;
            int register2 = -1;
            int register3 = -1;
            int imm = -1;

            while (++j < line_length) {
                const char *entry = array_get(line, j);
                int reg_val = register_lookup(entry);
                int const_val;
                int text_val;
                int data_val;
                if (reg_val != -1) {
                    if (register1 == -1) {
                        register1 = reg_val;
                    } else if (register2 == -1) {
                        register2 = reg_val;
                    } else {
                        register3 = reg_val;
                    }
                } else if (table_get(ctx->const_table, entry, &const_val)) {
                    imm = const_val;
                } else if (table_get(ctx->text_table, entry, &text_val)) {
                    imm = text_val - pc;
                } else if (table_get(ctx->data_table, entry, &data_val)) {
                    imm = data_val;
                } else {
                    imm = parse_value(entry);
                }
                
            }

            switch (instruction->type) {
            case R: {
                int hex = (instruction->funct7 << 25) + (register3 << 20) + (register2 << 15) + (instruction->funct3 << 12) + (register1 << 7) + instruction->opcode;
                fprintf(file, "%.08X\n", hex);
                break;
            }
            case I: {
                if (instruction->funct7 != -1) {
                    imm = (instruction->funct7 << 5) + imm;
                }

                if (strcmp(instruction->name, "ecall") == 0) {
                    register1 = 0;
                    register2 = 0;
                    imm = 0;
                } else if (strcmp(instruction->name, "ebreak") == 0) {
                    register1 = 0;
                    register2 = 0;
                    imm = 1;
                }

                int hex = ((imm & 0XFFF) << 20) + (register2 << 15) + (instruction->funct3 << 12) + (register1 << 7) + instruction->opcode;
                fprintf(file, "%.08X\n", hex);
                break;
            }
            case S: {
                int hex = ((imm & 0XFE0) << 20) + (register1 << 20) + (register2 << 15) + (instruction->funct3 << 12) + ((imm & 0X01F) << 7) + instruction->opcode;
                fprintf(file, "%.8X\n", hex);
                break;
            }
            case B: {
                int hex = ((imm & 0X1000) << 19) + ((imm & 0X7E0) << 20) + (register2 << 20) + (register1 << 15) + (instruction->funct3 << 12) + ((imm & 0X01E) << 7) + ((imm & 0X800) >> 4) + instruction->opcode;
                fprintf(file, "%.8X\n", hex);
                break;
            }
            case U: {
                int hex = ((imm & 0XFFFFF) << 12) + (register1 << 7) + instruction->opcode;
                fprintf(file, "%.8X\n", hex);
                break;
            }
            case J: {
                int hex = ((imm & 0X100000) << 11) + ((imm & 0X7FE) << 20) + ((imm & 0X800) << 9) + (imm & 0XFF000) + (register1 << 7) + instruction->opcode;
                fprintf(file, "%.8X\n", hex);
                break;
            }
            }
        } while (++i < length);
        fclose(file);
    }
}

static void get_substring(const char *str, int start, int end, char *dest) {
    int length = end + 1 - start;

    if (length < 1 || end > strlen(str) - 1) {
        fprintf(stderr, "Error: function get_substring either recieved an end before a start or an end past the length of str.\n");
        return;
    }

    strncpy(dest, str + start, length);
    dest[length] = '\0';

    return;
}

static void *intdup(int num) {
    int *nump = malloc(sizeof(num));

    if (!nump) {
        fprintf(stderr, "Error: intdup failed to allocate memory\n");
        exit(1);
    }

    *nump = num;
    return nump;
}

int compare_pointer_addresses(const void *a, const void *b) {
    const char *ptr1 = *(const char **)a;
    const char *ptr2 = *(const char **)b;

    uintptr_t addr1 = (uintptr_t)ptr1;
    uintptr_t addr2 = (uintptr_t)ptr2;

    if (addr1 < addr2) return -1;
    if (addr1 > addr2) return 1;
    return 0;
}

static void asm_init(asm_t *ctx, const char *filename) {
    ctx->filename = filename;
    ctx->assembly = array_create(4);
    ctx->clean_assembly = array_create(4);
    ctx->const_table = table_create(4);
    ctx->data_table = table_create(4);
    ctx->text_table = table_create(4);
    ctx->data_image = array_create(4);
    ctx->instructions = array_create(4);
}

static void asm_free(asm_t *ctx) {
    void * curr;
    int temp_length = array_get_size(ctx->clean_assembly) + array_get_size(ctx->instructions);
    array_t temp[temp_length];
    int temp_index = 0;

    while ((curr = array_pop(ctx->assembly))) {
        free(curr);
    }
    array_free(ctx->assembly);

    while ((curr = array_pop(ctx->clean_assembly))) {
        temp[temp_index] = curr;
        temp_index++;
    }

    while ((curr = array_pop(ctx->instructions))) {
        temp[temp_index] = curr;
        temp_index++;
    }

    qsort(temp, temp_length, sizeof(array_t), compare_pointer_addresses);

    for (int i = 0; i < temp_length; i++) {
        curr = temp[i];
        void *inner_curr;
        
        if (i > 0 && curr == temp[i-1]) {
            continue;
        }
        while ((inner_curr = array_pop(curr))) {
            free(inner_curr);
        }
        array_free(curr);
    }
    array_free(ctx->clean_assembly);
    array_free(ctx->instructions);

    table_free(ctx->const_table);
    table_free(ctx->data_table);
    table_free(ctx->text_table);

    while ((curr = array_pop(ctx->data_image))) {
        free(curr);
    }
    array_free(ctx->data_image);

    ctx->filename = NULL;
}

static void asm_dump(asm_t *ctx) {
    FILE *file = fopen("build/asm_dump.txt", "w");

    if (!file) {
        asm_error(ctx, "Error: Could not create or open build/asm_dump.txt");
    }

    fprintf(file, "--- asm_t memory dump ---\n");
    fprintf(file, "Source file: %s\n", ctx->filename);

    fprintf(file, "\n--- Printing Contents of ctx->assembly ---\n");
    for (int i = 0; i < array_get_size(ctx->assembly); i++) {
        fprintf(file, "%s\n", (char *) array_get(ctx->assembly, i));
    }

    fprintf(file, "\n--- Printing Contents of clean_assembly ---\n");
    for (int i = 0; i < array_get_size(ctx->clean_assembly); i++) {
        fprintf(file, "\n- Line %d -\n", i);
        const array_t temp = (array_t) array_get(ctx->clean_assembly, i);

        for (int j = 0; j < array_get_size(temp); j++) {
            fprintf(file, "%s\n", (char *) array_get(temp, j));
        }
    }

    fprintf(file, "\nconst_table:");
    table_print(ctx->const_table, file);

    fprintf(file, "\ndata_table:");
    table_print(ctx->data_table, file);

    fprintf(file, "\ntext_table:");
    table_print(ctx->text_table, file);

    fprintf(file, "\n--- Printing Contents of data_image ---\n");
    for (int i = 0; i < array_get_size(ctx->data_image); i = i + 4) {
        fprintf(file, "%02x %02x %02x %02x\n", *(int *) array_get(ctx->data_image, i), *(int *) array_get(ctx->data_image, i + 1), *(int *) array_get(ctx->data_image, i + 2), *(int *) array_get(ctx->data_image, i + 3));
    }

    fprintf(file, "\n--- Printing Contents of ctx->instructions ---\n");
    for (int i = 0; i < array_get_size(ctx->instructions); i++) {
        fprintf(file, "\n- Line %d -\n", i);
        const array_t temp = (array_t) array_get(ctx->instructions, i);

        for (int j = 0; j < array_get_size(temp); j++) {
            fprintf(file, "%s\n", (char *) array_get(temp, j));
        }
    }

    fclose(file);
}

static void asm_error(asm_t *ctx, const char *message) {
    fprintf(stderr, "Assembler Error: %s\n", message);

    if (ctx->file != NULL) {
        fclose(ctx->file);
        ctx->file = NULL;
    }

    exit(1);
}
