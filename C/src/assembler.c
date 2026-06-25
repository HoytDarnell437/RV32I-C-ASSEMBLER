#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/assembler.h"
#include "../include/dynamic_array.h"
#include "../include/table.h"
#include "../include/instruction.h"


// File macros
#define MAX_LINE_LENGTH 512

// Struct for assembler
typedef struct {
    const char *filename;
    int error;
    array_t assembly;
    char ***clean_assembly;
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

static void init_asm(asm_t *, const char *);
static void free_asm(asm_t *);

// Variables
static const pair_t register_table[] = {
    {"x0", 0}, {"zero", 0}, {"x1", 1}, {"ra", 1}, {"x2", 2}, {"sp", 2}, {"x3", 3}, {"gp", 3}, {"x4", 4}, {"tp", 4}, {"x5", 5}, {"t0", 5}, {"x6", 6}, {"t1", 6}, {"x7", 7}, {"t2", 7},
    {"x8", 8}, {"fp", 8}, {"s0", 8}, {"x9", 9}, {"s1", 9}, {"x10", 10}, {"a0", 10}, {"x11", 11}, {"a1", 11}, {"x12", 12}, {"a2", 12}, {"x13", 13}, {"a3", 13}, {"x14", 14}, {"a4", 14}, {"x15", 15}, {"a5", 15},
    {"x16", 16}, {"a6", 16}, {"x17", 17}, {"a7", 17}, {"x18", 18}, {"s2", 18}, {"x19", 19}, {"s3", 19}, {"x20", 20}, {"s4", 20}, {"x21", 21}, {"s5", 21}, {"x22", 22}, {"s6", 22}, {"x23", 23}, {"s7", 23},
    {"x24", 24}, {"s8", 24}, {"x25", 25}, {"s9", 25}, {"x26", 26}, {"s10", 26}, {"x27", 27}, {"s11", 27}, {"x28", 28}, {"t3", 28}, {"x29", 29}, {"t4", 29}, {"x30", 30}, {"t5", 30}, {"x31", 31}, {"t6", 31},
    {NULL}
};

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
    init_asm(&ctx, filename);

    // Read .asm file
    //read_assembly(&ctx);
    instruction_lookup(NULL);
    if (ctx.error){
        free_asm(&ctx);
        return ASM_FAILURE;
    }

    // TODO Implement next functions in assembler

    // Determine return value
    if (ctx.error){
        free_asm(&ctx);
        return ASM_FAILURE;
    }
    free_asm(&ctx);
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
        //buffer[strcspn(buffer, "\r\n")] = '\0';
        array_push(ctx->assembly, buffer);
    }
    // Push array termination entry to end of array
    array_push(ctx->assembly, NULL);

    array_print(ctx->assembly);

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
static void init_asm(asm_t *ctx, const char *filename){
    ctx->filename = filename;
    ctx->error = 0;
    ctx->assembly = array_create(4);
}

static void free_asm(asm_t *ctx){
    ctx->filename = NULL;
    array_free(ctx->assembly);
}

