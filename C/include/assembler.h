/**
 * @file assembler.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Dynamically allocated string array and 2D master array interfaces
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/**
 * @brief Reads the .asm file provided and produces data initialization and instruction initialization files.
 * @param filename Str of the .asm filename.
 */
void assemble(const char *filename);

#endif // ASSEMBLER_H