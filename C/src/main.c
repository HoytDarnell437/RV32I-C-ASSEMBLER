// C program to assemble riscv assembly and program a custom riscv core and monitor outputs

#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main(int argc, char *argv[]){
    // incorrect usage of executable
    if(argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    // assemble input file
    int assemble_out = assemble(argv[1]);

    // assembler encountered issue
    if (assemble_out == ASM_FAILURE){
        fprintf(stderr, "Error: Assembler encountered an error\n");
        return 1;
    }

    return 0;
}