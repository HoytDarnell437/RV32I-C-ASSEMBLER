#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
* @brief Read the .asm file provided and store it in a dynamic array of type array_t.
*
* Moves through the file appending each line to ctx->assembly.
*
* @param ctx Pointer to the active assembler context structure.
*/
int assemble(const char *);

#endif // ASSEMBLER_H