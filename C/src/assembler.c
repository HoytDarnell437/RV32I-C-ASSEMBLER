#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/assembler.h"
#include "../include/dynamic_array.h"
#include "../include/table.h"
#include "../include/instruction.h"
#include "../include/register.h"


// File macros
#define MAX_LINE_LENGTH 512

// Struct for assembler
typedef struct {
    const char *filename;
    int error;
    array_t assembly;
    master_array_t clean_assembly;
    //table_t const_table;
    //table_t data_table;
    char *data_image;
    char *instructions;
} asm_t;

// Core function prototypes
static void read_assembly(asm_t *);
static void format_assembly(asm_t *);
static void subroutine_gen(asm_t *);
static void create_data_file(const asm_t *);
static void isolate_instructions(asm_t *);
static void create_instruction_file(const asm_t *);

// Helper function prototypes
static void parse_value(char *);

static void asm_init(asm_t *, const char *);
static void asm_free(asm_t *);

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

    // Read .asm file
    read_assembly(&ctx);
    if (ctx.error){
        asm_free(&ctx);
        return ASM_FAILURE;
    }

    array_print(ctx.assembly);

    // Clean up the assembly
    //format_assembly(&ctx);
    if (ctx.error){
        asm_free(&ctx);
        return ASM_FAILURE;
    }

    // TODO Implement next functions in assembler

    // Determine return value
    if (ctx.error){
        asm_free(&ctx);
        return ASM_FAILURE;
    }
    asm_free(&ctx);
    return ASM_SUCCESS;
}

// Reads the assembly file
static void read_assembly(asm_t *ctx){
    // open given assembly file
    FILE *file = fopen(ctx->filename, "r");

    // File cannot be found
    if (!file){
        fprintf(stderr, "Error: Could not open file '%s'\n", ctx->filename);
        ctx->error = 1;
        return;
    }

    char buffer[MAX_LINE_LENGTH];

    // Read from file
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        buffer[strcspn(buffer, "\r\n")] = '\0';
        array_append(ctx->assembly, buffer);
    }

    //array_print(ctx->assembly);

    // Close the file
    fclose(file);
}

// Standardizes the riscv assembly to an array of lines which are arrays of their elements
static void format_assembly(asm_t *ctx){
    
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
static void parse_value(char *ctx){

}

// Assembler functions
static void asm_init(asm_t *ctx, const char *filename){
    ctx->filename = filename;
    ctx->error = 0;
    ctx->assembly = array_create(4);
    ctx->clean_assembly = master_array_create(4);
}

static void asm_free(asm_t *ctx){
    ctx->filename = NULL;
    array_free(ctx->assembly);
    master_array_free(ctx->clean_assembly);
}

