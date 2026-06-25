// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Last Updated: 06/25/2026
// Usage: This library provides the functionality of a linear lookup table consisting of instruction structs

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

// Enum for instruction type
typedef enum { R, I, S, B, U, J } instr_type_t;

// Struct for instructions
typedef struct {
    const char *name;
    const instr_type_t type;
    const int opcode;
    const int funct3;
    const int funct7;
} instruction_t;

// Public API functions
const instruction_t *instruction_lookup(const char *name); // Returns instruction type of given instruction

#endif // INSTRUCTION_H