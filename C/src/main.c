/**
 * @file main.c
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Assemble and bitstream assembly to FPGA
 */

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