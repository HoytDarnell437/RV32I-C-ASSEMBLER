#ifndef ASSEMBLER_H
#define ASSEMBLER_H

// status codes returned by assemble function
#define ASM_SUCCESS 0
#define ASM_FAILURE 1

// main function takes in a filename and produces machine code and a data initialization image
int assemble(const char *);

#endif // ASSEMBLER_H