// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Last Updated: 06/25/2026
// Usage: This library provides the functionality of a dynamically allocated linear lookup table consisting of pairs with a str key and int value

#ifndef TABLE_H
#define TABLE_H

// Pair type
typedef struct {
    char *key;
    int value; 
} pair_t;

// Table type
struct table_impl;
typedef struct table_impl* table_t;

// Public API functions
table_t table_create(int initial_capacity); // Create and initialize table variable 
void table_free(table_t table); // Deallocates the memory used by the table
void table_print(table_t table); // Prints the contents of the table to terminal

void table_set(table_t table, const char *key, int value); // Adds a pair to the table or updates existing key to new value
int table_get(const table_t table, const char *key, int *value); // Retrieves the value associated with the key

#endif // TABLE_H