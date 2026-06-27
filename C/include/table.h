/**
 * @file table.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Linear lookup table with string keys and integer values.
 */

#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>

typedef struct {
    char *key;
    int value; 
} pair_t;

struct table_impl;
typedef struct table_impl* table_t;

/**
 * @brief Creates a table_t type.
 * @param initial_capacity Initial capacity of created table.
 * @return Returns a pointer to the created table.
 */
table_t table_create(int initial_capacity);

/**
 * @brief Frees all elements of a table_t type.
 * @param table Pointer to table to free.
 */
void table_free(table_t table);

/**
 * @brief Prints the contents of a table.
 * @param table Pointer to table object.
 * @param file File to print contents to.
 */
void table_print(const table_t table, FILE *file);

/**
 * @brief Sets the value of key in the table.
 * @param table Pointer to table_t object.
 * @param key Key of table in which the value will be changed.
 * @param value Value to change key to be associated with.
 */
void table_set(table_t table, const char *key, int value);

/**
 * @brief Retrieves the value of a table associated with key.
 * @param table Pointer to table object.
 * @param key Key to retrieve value from the table.
 * @param value Pointer where the retrieved value will be stored.
 * @return Returns 1 if key is found 0 if not
 */
int table_get(const table_t table, const char *key, int *value);

#endif // TABLE_H