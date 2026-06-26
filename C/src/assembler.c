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

*/


// File macros
#define MAX_LINE_LENGTH 512

// Struct for assembler
typedef struct {
    const char *filename;
    FILE *file;
    array_t assembly;
    master_array_t clean_assembly;
    table_t const_table;
    table_t data_table;
    table_t text_table;
    char *data_image;
    char *instructions;
} asm_t;

// Core function prototypes
static void read_assembly(asm_t *ctx);
static void format_assembly(asm_t *ctx);
static void subroutine_gen(asm_t *ctx);
static void create_data_file(const asm_t *ctx);
static void isolate_instructions(asm_t *ctx);
static void create_instruction_file(const asm_t *ctx);

// Helper function prototypes
static void parse_value(char *str);

// asm_t member function protoypes
static void asm_init(asm_t *ctx, const char *filename);
static void asm_free(asm_t *ctx);
static void asm_dump(asm_t *ctx);
static void asm_error(asm_t *ctx, const char *message);

// Variables
static const pair_t escape_table[] = {
    {"\\n",  0x0A}, {"\\t", 0x09}, {"\\r",  0x0D}, {"\\0", 0x00},
    {"\\\\", 0x5C}, {"\\'", 0x27}, {"\\\"", 0x22}, {"\\b", 0x08},
    {"\\f",  0x0C}, {"\\a", 0x07}, {"\\v",  0x0B},
    {NULL}
};

// Main function
int assemble(const char *filename){
    // Declare new asm_t variable and set its default values
    asm_t ctx;
    asm_init(&ctx, filename);

    read_assembly(&ctx); // Read .asm file

    format_assembly(&ctx); // Clean up the assembly

    // TODO 1

    asm_free(&ctx);
    return ASM_SUCCESS;
}

// Reads the assembly file
static void read_assembly(asm_t *ctx){
    ctx->file = fopen(ctx->filename, "r"); // Open given assembly file

    if (!ctx->file){ // File cannot be found
        asm_error(ctx, "Error: Could not open .asm file");
    }

    char buffer[MAX_LINE_LENGTH]; // Line buffer

    while (fgets(buffer, sizeof(buffer), ctx->file) != NULL){ // Read from file
        buffer[strcspn(buffer, "\r\n")] = '\0';
        array_append(ctx->assembly, buffer);
    }

    array_append(ctx->assembly, NULL); // Append sentinel character to assembly array

    fclose(ctx->file);
}

// Standardizes the riscv assembly to a master array of lines which are arrays of strings
static void format_assembly(asm_t *ctx){
    for (int i = 0; array_get(ctx->assembly, i) != NULL ; i++){
        char *tok = strtok(array_get(ctx->assembly, i), " ,()");
        array_t sub_array = array_create(1);
        while (tok != NULL){
            if (tok[0] == '#'){ // Remove comments
                break;
            }

            array_append(sub_array, tok); // Append token to sub_array

            if (strchr(tok, ':') != NULL || strchr(tok, '.') != NULL){ // Separate same line labels and directives
                master_array_append(ctx->clean_assembly, array_dupe(sub_array)); // append label as own array
                free(array_pop(sub_array));
            }

            tok = strtok(NULL, " ,()"); // Get next token
        }

        if (array_get_size(sub_array) != 0 && strchr(array_get(sub_array, 0), ':') == NULL && strchr(array_get(sub_array, 0), '.') == NULL){
            master_array_append(ctx->clean_assembly, sub_array); // Append instructions
        }
    }
    master_array_print(ctx->clean_assembly); // Print array for debugging
}

// Generates arrays of labels and their addresses
static void subroutine_gen(asm_t *ctx){

}

// Creates data initialization file
static void create_data_file(const asm_t *ctx){

}

// Isolates instructions
static void isolate_instructions(asm_t *ctx){

}

// Creates instruction initialization file
static void create_instruction_file(const asm_t *ctx){

}

// Helper functions
static void parse_value(char *str){

}

// Assembler functions
static void asm_init(asm_t *ctx, const char *filename){
    ctx->filename = filename;
    ctx->assembly = array_create(4);
    ctx->clean_assembly = master_array_create(4);
    ctx->const_table = table_create(4);
    ctx->data_table = table_create(4);
    ctx->text_table = table_create(4);
}

static void asm_free(asm_t *ctx){
    ctx->filename = NULL;
    array_free(ctx->assembly);
    master_array_free(ctx->clean_assembly);
    table_free(ctx->const_table);
    table_free(ctx->data_table);
    table_free(ctx->text_table);
}

static void asm_dump(asm_t *ctx){
    FILE *file = fopen(strcat(ctx->filename, "_asm_dump.txt"), "w");
    
}

static void asm_error(asm_t *ctx, const char *message){
    fprintf(stderr, "Assembler Error: %s\n", message);
    if (ctx->file != NULL){
        fclose(ctx->file);
    }
    exit(1);
}