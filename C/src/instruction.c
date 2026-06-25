// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Implementation for instruction.h

#include <string.h>
#include "../include/instruction.h"

// Table
static const instruction_t instruction_table[] = {
    // U-Type
    {"lui", U, 0x37, -1, -1}, {"auipc", U, 0x17, -1, -1},
    // J-Type
    {"jal", J, 0x6f, -1, -1}, {"j",     J, 0x6f, -1, -1},
    // B-Type
    {"beq", B, 0x63, 0x0, -1}, {"bne",  B, 0x63, 0x1, -1}, {"blt",  B, 0x63, 0x4, -1},
    {"bge", B, 0x63, 0x5, -1}, {"bltu", B, 0x63, 0x6, -1}, {"bgeu", B, 0x63, 0x7, -1},
    // S-Type
    {"sb",  S, 0x23, 0x0, -1}, {"sh", S, 0x23, 0x1, -1}, {"sw", S, 0x23, 0x2, -1},
    // I-Type
    {"jalr", I, 0x67, 0x0, -1},
    {"lb",   I, 0x03, 0x0, -1}, {"lh",   I, 0x03, 0x1, -1}, {"lw",     I, 0x03, 0x2, -1},
    {"lbu",  I, 0x03, 0x4, -1}, {"lhu",  I, 0x03, 0x5, -1},
    {"addi", I, 0x13, 0x0, -1}, {"slti", I, 0x13, 0x2, -1}, {"sltiu",  I, 0x13, 0x3, -1},
    {"xori", I, 0x13, 0x4, -1}, {"ori",  I, 0x13, 0x6, -1}, {"andi",   I, 0x13, 0x7, -1},
    {"slli", I, 0x13, 0x1, 0x0}, {"srli", I, 0x13, 0x5, 0x0}, {"srai", I, 0x13, 0x5, 0x20},
    // R-Type
    {"add", R, 0x33, 0x0, 0x0}, {"sub",  R, 0x33, 0x0, 0x20}, {"sll", R, 0x33, 0x1, 0x0},
    {"slt", R, 0x33, 0x2, 0x0}, {"sltu", R, 0x33, 0x3, 0x0}, {"xor",  R, 0x33, 0x4, 0x0},
    {"srl", R, 0x33, 0x5, 0x0}, {"sra",  R, 0x33, 0x5, 0x20}, {"or",  R, 0x33, 0x6, 0x0},
    {"and", R, 0x33, 0x7, 0x0},
    // System & Memory Fences
    {"fence", I, 0x0f, 0x0, -1}, {"fence.i", I, 0x0f, 0x1, -1},
    {"ecall", I, 0x73, 0x0, -1}, {"ebreak",  I, 0x73, 0x0, -1},

    {NULL}
};

// Functions
const instruction_t *instruction_lookup(const char *name){
    for (int i = 0; i < 42; i++){
        instruction_print(instruction_table[i]);
    }
}

const void instruction_print(instruction_t instruction){
    printf("%s | Opcode: %x", instruction.name, instruction.opcode);
    if (instruction.funct3 != -1){
        printf(" | funct3: %x", instruction.funct3);
    }
    if (instruction.funct7 != -1){
        printf(" | funct7: %x", instruction.funct7);
    }
    printf("\n");
}