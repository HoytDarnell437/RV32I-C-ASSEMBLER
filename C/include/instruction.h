/**
 * @file instruction.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Instruction type and table for easy lookup.
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef enum { R, I, S, B, U, J } instr_type_t;

typedef struct {
    const char *name;
    const instr_type_t type;
    const int opcode;
    const int funct3;
    const int funct7;
} instruction_t;

/**
 * @brief Returns the instruction_t of a given RV32I instruction.
 * @param name Name of RV32I instruction.
 * @return Returns the instruction_t of the provided instruction.
 */
const instruction_t *instruction_lookup(const char *name);

#endif // INSTRUCTION_H