/**
 * @file assembler.c
 * @brief Implementation for assembler.h
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/assembler.h"
#include "../include/dynamic_array.h"
#include "../include/instruction.h"
#include "../include/register.h"
#include "../include/table.h"

#define MAX_LINE_LENGTH 512

/**
 * @struct asm_t
 * @brief Stores the context for the assembler.
 */
typedef struct {
    const char *filename;               /**< Name of .asm file. */
    FILE *file;                         /**< Pointer to .asm file. */
    char_array_t assembly;              /**< Array of the .asm file's lines. */
    master_char_array_t clean_assembly; /**< 2D Master array of the .asm file's tokens with comments removed. */
    table_t data_table;                 /**< Table with the data labels as keys and their addresses as corresponding values. */
    table_t text_table;                 /**< Table with the text labels as keys and their addresses as corresponding values. */
    table_t const_table;                /**< Table with constant labels as the keys and their corresponding values. */
    int_array_t data_image;             /**< Array of data initialization contents. */
    master_char_array_t instructions;   /**< Array of instructions. */
} asm_t;

/* asm_t Associated Functions */

/**
 * @brief Initializes all members of ctx.
 * @param ctx Pointer to the active assembler context structure.
 * @param[in] filename Name of .asm file.
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

/* Assembly Parsing Pipeline */

/**
 * @brief Read the .asm file provided and store it in a dynamic array of type char_array_t.
 * @param ctx Pointer to the active assembler context structure.
 */
static void read_assembly(asm_t *ctx);

/**
 * @brief Tokenize raw assembly lines into a structured 2D array.
 * Separates lines into separate tokens, removes comments, separates same line labels, and forces lowercase letters.
 * @param ctx Pointer to the active assembler context structure.
 */
static void format_assembly(asm_t *ctx);

/**
 * @brief Take in the structured assembly and produce a constant value table, a table of instruction labels and their addresses,
 * a table of data labels and their addresses, an array of bytes making up the initial data image, and a list containing the instructions.
 *  @param ctx Pointer to the active assembler context structure.
 */
static void subroutine_gen(asm_t *ctx);

/* Output Generators */

/**
 * @brief Takes in a data image and writes the contents to a file named data.hex.
 * @param ctx Pointer to the active assembler context structure.
 */
static void create_data_file(asm_t *ctx);

/**
 * @brief Uses the master_array of instructions to create a file named instructions.hex.
 * @param ctx pointer to the active assembler context structure.
 */
static void create_instruction_file(asm_t *ctx);

/* Helper Functions */
/**
 * @brief Convert strings into their ascii values.
 * @param str String to parse the value of.
 */
static int parse_value(const char *str);

/**
 * @brief Convert strings of decimal numbers into integers.
 * @param str String to convert.
 */
static int dec_str_int(const char *str);

/**
 * @brief Convert strings of hexadecimal numbers into integers.
 * @param str String to convert.
 */
static int hex_str_int(const char *str);

/**
 * @brief Copy a string starting at one index and ending on another (inclusive).
 * @param str String to copy from.
 * @param start Index to start on.
 * @param end Index to end on (inclusive).
 * @param dest String to copy to.
 */
static void get_substring(const char *str, int start, int end, char *dest);

void assemble(const char *filename) {
    asm_t ctx;
    asm_init(&ctx, filename);

    read_assembly(&ctx);
    format_assembly(&ctx);
    subroutine_gen(&ctx);

    create_data_file(&ctx);
    create_instruction_file(&ctx);
    
    // TODO

    asm_dump(&ctx);

    asm_free(&ctx);
}

static void read_assembly(asm_t *ctx) {
    ctx->file = fopen(ctx->filename, "r");
    char buffer[MAX_LINE_LENGTH];

    if (!ctx->file) {
        asm_error(ctx, "Error: Could not open .asm file");
    }

    while (fgets(buffer, sizeof(buffer), ctx->file) != NULL) {
        size_t len = strlen(buffer);

        if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n' && !feof(ctx->file)) {
            asm_error(ctx, "Error: Line in assembly file exceeds MAX_LINE_LENGTH");
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
        char_array_append(ctx->assembly, buffer);
    }

    char_array_append(ctx->assembly, NULL);
    fclose(ctx->file);
}

static void format_assembly(asm_t *ctx) {
    for (int i = 0; char_array_get(ctx->assembly, i) != NULL; i++) {
        char *str = strdup(char_array_get(ctx->assembly, i));
        char *tok = strtok(str, " ,()");
        char_array_t sub_array = char_array_create(1);

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
                char_array_append(sub_array, "' '");
                tok = strtok(NULL, " ,()");
            } else {
                // Note: array_append handles deep-copying internally via strdup
                char_array_append(sub_array, tok);
            }

            if (strchr(tok, ':') != NULL && strstr(char_array_get(ctx->assembly, i), ".equ") == NULL) {
                master_array_append(ctx->clean_assembly, char_array_dupe(sub_array));
                free(char_array_pop(sub_array));
            }

            tok = strtok(NULL, " ,()");
        }

        if (char_array_get_size(sub_array) != 0) {
            master_array_append(ctx->clean_assembly, sub_array);
        } else {
            char_array_free(sub_array);
        }

        free(str);
    }
}

static void subroutine_gen(asm_t *ctx) {
    const char *directive = ".text";
    int text_counter = 0;
    int data_counter = 2048;

    for (int i = 0; i < master_array_get_size(ctx->clean_assembly); i++) {
        char_array_t line = master_array_get(ctx->clean_assembly, i);
        const char *string = char_array_get(line, 0);

        if (strcmp(string, ".text") == 0) {
            directive = ".text";
        } else if (strcmp(string, ".data") == 0) {
            directive = ".data";
        } else if (strcmp(string, ".word") == 0) {
            for (int j = 1; j < char_array_get_size(line); j++) {
                const char *tok = char_array_get(line, j);
                int value = parse_value(tok);

                if (int_array_get_size(ctx->data_image) & 0b11) {
                    int count = int_array_get_size(ctx->data_image) & 0b11;

                    do {
                        int_array_append(ctx->data_image, 0);
                        data_counter += 1;
                    } while (++count < 4);

                    const char *label = char_array_get(master_array_get(ctx->clean_assembly, i - 1), 0);

                    if (table_get(ctx->data_table, label, NULL) == 1) {
                        table_set(ctx->data_table, label, data_counter);
                    }
                }

                int_array_append(ctx->data_image, value & 0x000000ff);
                int_array_append(ctx->data_image, value >> 8 & 0x000000ff);
                int_array_append(ctx->data_image, value >> 16 & 0x000000ff);
                int_array_append(ctx->data_image, value >> 24 & 0x000000ff);
                data_counter += 4;
            }

        } else if (strcmp(string, ".byte") == 0) {
            for (int j = 1; j < char_array_get_size(line); j++) {
                const char *tok = char_array_get(line, j);
                int value = 0;
                value += parse_value(tok);
                int_array_append(ctx->data_image, value);
                data_counter += 1;
            }

        } else if (strcmp(string, ".space") == 0) {
            int space = parse_value(char_array_get(line, 1));

            for (int j = 0; j < space; j++) {
                int_array_append(ctx->data_image, 0);
            }

            data_counter += space;

        } else if (strcmp(string, ".align") == 0) {
            int align_to = 1 << parse_value(char_array_get(line, 1));

            if (align_to >= 2) {
                int remainder = data_counter % align_to;

                if (remainder > 0) {

                    int space = align_to - remainder;

                    for (int j = 0; j < space; j++) {
                        int_array_append(ctx->data_image, 0);
                    }

                    data_counter += space;
                }
            }
        } else if (char_array_get_size(line) > 1 && strcmp(char_array_get(line, 1), ".equ") == 0) {
            const char *label = string;
            char clean_label[32];
            get_substring(label, 0, strlen(label) - 2, clean_label);
            table_set(ctx->const_table, clean_label, parse_value(char_array_get(line, 2)));

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
            text_counter += 4;
            char_array_t sub_array = char_array_dupe(line);
            master_array_append(ctx->instructions, sub_array);
        }
    }
}

static void create_data_file(asm_t *ctx) {
    FILE *file = fopen("build/data.hex", "w");
    int line_count = int_array_get_size(ctx->data_image);

    if (!file) {
        asm_error(ctx, "Error: could not open build/data.hex\n");
    }

    if (line_count < 1) {
        fclose(file);
    } else {
        for (int i = 0; i < line_count; i++) {
            fprintf(file, "%02x", int_array_get(ctx->data_image, i));

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

    if (master_array_get_size(ctx->instructions) < 1) {
        fclose(file);
        asm_error(ctx, "Error: Assembly given to assembler must have atleast one instruction\n");

    } else {
        int length = master_array_get_size(ctx->instructions);
        int pc = -4;
        int i = 0;

        do {
            const instruction_t *instruction;
            char_array_t line;
            const char *mnemonic;

            line = master_array_get(ctx->instructions, i);
            int line_length = char_array_get_size(line);
            mnemonic = char_array_get(line, 0);
            instruction = instruction_lookup(mnemonic);
            pc += 4;

            int j = 0;
            int register1 = -1;
            int register2 = -1;
            int register3 = -1;
            int imm = -1;

            while (++j < line_length) {
                const char *entry = char_array_get(line, j);
                int reg_val = register_lookup(entry);
                int const_val;
                int text_val;
                int data_val;
                printf("%s\n", entry);
                if (reg_val != -1) {
                    printf("register\n");
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
                int hex = ((imm & 0X100000) << 11) + ((imm & 0X7FE) << 20) + ((imm & 0X800) << 9) + (imm & 0XFE000) + (register1 << 7) + instruction->opcode;
                fprintf(file, "%.8X\n", hex);
                break;
            }
            }
        } while (++i < length);
        fclose(file);
    }
}

static int parse_value(const char *str) {
    if (strlen(str) < 1) {
        fprintf(stderr, "Error: Impropper input to parse_value function: NULL or Empty str\n");
        return (0);
    }

    if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
        if (str[1] == '\\') {
            switch (str[2]) {
            case '0':
                return 0;
            case 'n':
                return 10;
            case 't':
                return 9;
            case '\\':
                return 92;
            case '\'':
                return 39;
            }
        }

        return str[1];
    }

    int value = 0;

    if (str[0] == '0' && str[1] == 'x') {
        if (str[2] == '\0') {
            fprintf(stderr, "Error: Empty hex constant given to parse_value function \"0x\"\n");
        }

        value = hex_str_int(str);

    } else {
        value = dec_str_int(str);
    }

    return value;
}

static int dec_str_int(const char *str) {
    int value = 0;
    int negative = 0;
    int i = 0;

    if (str[0] == '-') {
        negative = 1;
        i++;
    }

    do {
        value *= 10;
        value += ((int)str[i] - 48);
    } while (str[++i] != '\0');

    if (negative) {
        value = -value;
    }

    return value;
}

static int hex_str_int(const char *str) {
    int value = 0;
    int i = 2;

    do {
        value *= 16;
        if (str[i] < 58) {
            value += ((int)str[i] - 48);
        } else {
            value += ((int)str[i] - 87);
        }
    } while (str[++i] != '\0');

    return value;
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

static void asm_init(asm_t *ctx, const char *filename) {
    ctx->filename = filename;
    ctx->assembly = char_array_create(4);
    ctx->clean_assembly = master_array_create(4);
    ctx->const_table = table_create(4);
    ctx->data_table = table_create(4);
    ctx->text_table = table_create(4);
    ctx->data_image = int_array_create(4);
    ctx->instructions = master_array_create(4);
}

static void asm_free(asm_t *ctx) {
    char_array_free(ctx->assembly);
    master_array_free(ctx->clean_assembly);
    table_free(ctx->const_table);
    table_free(ctx->data_table);
    table_free(ctx->text_table);
    int_array_free(ctx->data_image);
    master_array_free(ctx->instructions);

    ctx->filename = NULL;
}

static void asm_dump(asm_t *ctx) {
    FILE *file = fopen("build/asm_dump.txt", "w");

    if (!file) {
        asm_error(ctx, "Error: Could not create or open build/asm_dump.txt");
    }

    fprintf(file, "--- asm_t memory dump ---\n");
    fprintf(file, "Source file: %s\n", ctx->filename);

    fprintf(file, "\nassembly:");
    char_array_print(ctx->assembly, file);

    fprintf(file, "\nclean_assembly:");
    master_array_print(ctx->clean_assembly, file);

    fprintf(file, "\nconst_table:");
    table_print(ctx->const_table, file);

    fprintf(file, "\ndata_table:");
    table_print(ctx->data_table, file);

    fprintf(file, "\ntext_table:");
    table_print(ctx->text_table, file);

    fprintf(file, "\ndata_image:");
    int_array_print(ctx->data_image, file);

    fprintf(file, "\ninstructions:");
    master_array_print(ctx->instructions, file);

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
