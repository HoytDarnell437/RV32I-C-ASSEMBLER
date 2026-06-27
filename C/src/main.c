// C program to assemble riscv assembly and program a custom riscv core and monitor outputs

#include <stdio.h>
#include <stdlib.h>

#include "../include/assembler.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    assemble(argv[1]);

    return 0;
}