#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
* @brief Reads the .asm file provided and produces data initialization and instruction initialization files.
*
*
* @param filename Pointer to the active assembler context structure.
* @return 
*/
void assemble(const char *filename);

#endif // ASSEMBLER_H