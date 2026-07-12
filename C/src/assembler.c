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
#include "../include/table.h"
// #include "../include/instruction.h"
// #include "../include/register.h"

/*

--- TODO List ---
1. Implement Remaining Functions
2. Rewrite doxygen comments to be concise
*/

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
    char *instructions;                 /**< Array of instructions. */
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

static void subroutine_gen(asm_t *ctx);
static void isolate_instructions(asm_t *ctx);

/* Output Generators */
static void create_data_file(const asm_t *ctx);
static void create_instruction_file(const asm_t *ctx);

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

    // TODO 1

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

            if (strchr(tok, ':') != NULL) {
                master_array_append(ctx->clean_assembly, char_array_dupe(sub_array));
                free(char_array_pop(sub_array));
            }

            tok = strtok(NULL, " ,()");
        }

        if (char_array_get_size(sub_array) != 0 &&
            strchr(char_array_get(sub_array, 0), ':') == NULL) {
            master_array_append(ctx->clean_assembly, sub_array);
        } else {
            char_array_free(sub_array);
        }

        free(str);
    }
}

static void subroutine_gen(asm_t *ctx) {
    char *directive = ".text";
    int text_counter = 0;
    int data_counter = 2048;

    for (int i = 0; i < master_array_get_size(ctx->clean_assembly); i++) {
        char_array_t line = master_array_get(ctx->clean_assembly, i);
        char *string = char_array_get(line, 0);

        if (strcmp(string, ".text") == 0) {
            directive = ".text";
        } else if (strcmp(string, ".data") == 0) {
            directive = ".data";
        } else if (strcmp(string, ".word") == 0) {
            for (int j = 1; j < char_array_get_size(line); j++) {
                const char *tok = char_array_get(line, j);
                int value = parse_value(tok);
                printf("Parsed Value .word: %d\n", value);
            }
        }
    }
}

static void create_data_file(const asm_t *ctx) {}

static void isolate_instructions(asm_t *ctx) {}

static void create_instruction_file(const asm_t *ctx) {}

static int parse_value(const char *str) {
    if (strlen(str) < 1) {
        fprintf(stderr, "Error: Impropper input to parse_value function: NULL or Empty str\n");
        return (0);
    }

    if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
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
}

static void asm_free(asm_t *ctx) {
    char_array_free(ctx->assembly);
    master_array_free(ctx->clean_assembly);
    table_free(ctx->const_table);
    table_free(ctx->data_table);
    table_free(ctx->text_table);

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
