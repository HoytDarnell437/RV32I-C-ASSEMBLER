// C program to assemble riscv assembly and program a custom riscv core and monitor outputs

#include <stdio.h>
#include <stdlib.h>

#include "../include/assembler.h"

int main(int argc, char *argv[]){
    // incorrect usage of executable
    if(argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    // assemble input file
    assemble(argv[1]);

    return 0;
}