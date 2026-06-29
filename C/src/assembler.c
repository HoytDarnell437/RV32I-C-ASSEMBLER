/**
 * @file assembler.c
 * @brief Implementation for assembler.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/assembler.h"
#include "../include/dynamic_array.h"
#include "../include/table.h"
#include "../include/instruction.h"
#include "../include/register.h"

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
    const char *filename; /**< Name of .asm file. */
    FILE *file; /**< Pointer to .asm file. */
    array_t assembly; /**< Array of the .asm file's lines. */
    master_array_t clean_assembly; /**< 2D Master array of the .asm file's tokens with comments removed. */
    table_t data_table; /**< Table with the data labels as keys and their addresses as corresponding values. */
    table_t text_table; /**< Table with the text labels as keys and their addresses as corresponding values. */
    table_t const_table; /**< Table with constant labels as the keys and their corresponding values. */
    char *data_image; /**< Array of data initialization contents. */
    char *instructions; /**< Array of instructions. */
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
 * @brief Read the .asm file provided and store it in a dynamic array of type array_t.
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

/* Helper Function */
static void parse_value(char *str); 

static const pair_t escape_table[] = {
    {"\\n",  0x0A}, {"\\t", 0x09}, {"\\r",  0x0D}, {"\\0", 0x00},
    {"\\\\", 0x5C}, {"\\'", 0x27}, {"\\\"", 0x22}, {"\\b", 0x08},
    {"\\f",  0x0C}, {"\\a", 0x07}, {"\\v",  0x0B},
    {NULL}
};

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
    size_t len;

    if (!ctx->file) {
        asm_error(ctx, "Error: Could not open .asm file");
    }

    while (fgets(buffer, sizeof(buffer), ctx->file) != NULL) {
        len = strlen(buffer);

        if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n' && !feof(ctx->file)) {
            asm_error(ctx, "Error: Line in assembly file exceeds MAX_LINE_LENGTH");
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';
        array_append(ctx->assembly, buffer);
    }

    array_append(ctx->assembly, NULL);
    fclose(ctx->file);
}

static void format_assembly(asm_t *ctx) {
    for (int i = 0; array_get(ctx->assembly, i) != NULL; i++) {
        char *str = strdup(array_get(ctx->assembly, i));
        char *tok = strtok(str, " ,()");
        array_t sub_array = array_create(1);

        while (tok != NULL) {
            if (tok[0] == '#') {
                break;
            }

            for (int j = 0; tok[j] != '\0'; j++) {
                if (tok[j] < 91 && tok[j] > 64) {
                    tok[j] += 32;
                }
            }

            // Note: array_append handles deep-copying internally via strdup
            array_append(sub_array, tok);

            if (strchr(tok, ':') != NULL) { 
                master_array_append(ctx->clean_assembly, array_dupe(sub_array));
                free(array_pop(sub_array));
            }

            tok = strtok(NULL, " ,()");
        }

        if (array_get_size(sub_array) != 0 && strchr(array_get(sub_array, 0), ':') == NULL) {
            master_array_append(ctx->clean_assembly, sub_array);
        } else {
            array_free(sub_array);
        }

        free(str);
    }
}

static void subroutine_gen(asm_t *ctx) {
    ctx->data_table = table_create(4);
    ctx->text_table = table_create(4);
    ctx->const_table = table_create(4);
    //data_image

    char *directive = ".text";
    int text_counter = 0;
    int data_counter = 2048;

    for (int i = 0; i < master_array_get_size(ctx->clean_assembly); i++) {
        if(master_array_get(ctx->clean_assembly, i))
    }
}

static void create_data_file(const asm_t *ctx) {

}

static void isolate_instructions(asm_t *ctx) {

}

static void create_instruction_file(const asm_t *ctx) {

}

static void parse_value(char *str) {

}

static void asm_init(asm_t *ctx, const char *filename) {
    ctx->filename = filename;
    ctx->assembly = array_create(4);
    ctx->clean_assembly = master_array_create(4);
    ctx->const_table = table_create(4);
    ctx->data_table = table_create(4);
    ctx->text_table = table_create(4);
}

static void asm_free(asm_t *ctx) {
    array_free(ctx->assembly);
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
    array_print(ctx->assembly, file);

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