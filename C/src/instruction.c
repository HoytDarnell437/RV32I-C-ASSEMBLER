/**
 * @file instruction.c
 * @brief Implementation for instruction.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/instruction.h"

/**
 * @brief Static lookup table containing the RV32I instruction set architecture.
 * @note Sorted by instruction type; terminated with a '{NULL}' sentinel entry.
 */
static const instruction_t instruction_table[] = {

    {"lui", U, 0x37, -1, -1}, {"auipc", U, 0x17, -1, -1},

    {"jal", J, 0x6f, -1, -1}, {"j",     J, 0x6f, -1, -1},

    {"beq", B, 0x63, 0x0, -1}, {"bne",  B, 0x63, 0x1, -1}, {"blt",  B, 0x63, 0x4, -1},
    {"bge", B, 0x63, 0x5, -1}, {"bltu", B, 0x63, 0x6, -1}, {"bgeu", B, 0x63, 0x7, -1},

    {"sb",  S, 0x23, 0x0, -1}, {"sh", S, 0x23, 0x1, -1}, {"sw", S, 0x23, 0x2, -1},

    {"jalr", I, 0x67, 0x0, -1},
    {"lb",   I, 0x03, 0x0, -1}, {"lh",   I, 0x03, 0x1, -1}, {"lw",     I, 0x03, 0x2, -1},
    {"lbu",  I, 0x03, 0x4, -1}, {"lhu",  I, 0x03, 0x5, -1},
    {"addi", I, 0x13, 0x0, -1}, {"slti", I, 0x13, 0x2, -1}, {"sltiu",  I, 0x13, 0x3, -1},
    {"xori", I, 0x13, 0x4, -1}, {"ori",  I, 0x13, 0x6, -1}, {"andi",   I, 0x13, 0x7, -1},
    {"slli", I, 0x13, 0x1, 0x0}, {"srli", I, 0x13, 0x5, 0x0}, {"srai", I, 0x13, 0x5, 0x20},

    {"add", R, 0x33, 0x0, 0x0}, {"sub",  R, 0x33, 0x0, 0x20}, {"sll", R, 0x33, 0x1, 0x0},
    {"slt", R, 0x33, 0x2, 0x0}, {"sltu", R, 0x33, 0x3, 0x0}, {"xor",  R, 0x33, 0x4, 0x0},
    {"srl", R, 0x33, 0x5, 0x0}, {"sra",  R, 0x33, 0x5, 0x20}, {"or",  R, 0x33, 0x6, 0x0},
    {"and", R, 0x33, 0x7, 0x0},

    {"fence", I, 0x0f, 0x0, -1}, {"fence.i", I, 0x0f, 0x1, -1},
    {"ecall", I, 0x73, 0x0, -1}, {"ebreak",  I, 0x73, 0x0, -1},

    {NULL}
};

static void instruction_print(const instruction_t instruction);


const instruction_t *instruction_lookup(const char *name) {
    if (name != NULL) {

        for (int i = 0; instruction_table[i].name != NULL; i++) {
            if (strcmp(instruction_table[i].name, name) == 0) {
                return &instruction_table[i];
            }
        }
    }

    fprintf(stderr, "Error: passed invalid instruction to instruction_lookup '%s'\n", name);
    exit(1);
}

static void instruction_print(const instruction_t instruction) {
    printf("%s | Opcode: 0x%x", instruction.name, instruction.opcode);

    if (instruction.funct3 != -1) {
        printf(" | funct3: 0x%x", instruction.funct3);
    }

    if (instruction.funct7 != -1) {
        printf(" | funct7: 0x%x", instruction.funct7);
    }

    printf("\n");
}