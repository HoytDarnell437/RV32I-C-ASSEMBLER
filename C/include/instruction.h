/**
 * @file instruction.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Instruction type and table for easy lookup.
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

/**
 * @enum instr_type_t
 * @brief Stores the different instruction types.
 */
typedef enum { 
    R, /**< R-type instruction */
    I, /**< I-type instruction */
    S, /**< S-type instruction */
    B, /**< B-type instruction */
    U, /**< U-type instruction */
    J  /**< J-type instruction */
} instr_type_t;

/**
 * @struct instruction_t
 * @brief Stores the data for the instructions.
 */
typedef struct {
    const char *name; /**< Name of the RV32I instruction. */
    const instr_type_t type; /**< Instruction type. */
    const int opcode; /**< Instruction opcode. */
    const int funct3; /**< Instruction funct3. */
    const int funct7; /**< Instruction funct7. */
} instruction_t;

/**
 * @brief Returns the instruction_t of a given RV32I instruction.
 * @param name Name of RV32I instruction.
 * @return Returns the instruction_t of the provided instruction.
 */
const instruction_t *instruction_lookup(const char *name);

#endif // INSTRUCTION_H