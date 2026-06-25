// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Implementation for table.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/table.h"

// Struct for lookup table
typedef struct table_impl{
    int capacity;
    int size;
    pair_t *pairs;
} table_impl_t;

// Function prototypes
static pair_t pair_create(const char *key, int value);
static void pair_print(pair_t pair);

// Table functions
table_t table_create(int initial_capacity){
    if (initial_capacity <= 0){
        fprintf(stderr, "Error: table_create received initial_capacity less than 1\n");
        exit(1);
    }

    table_t table = malloc(sizeof(table_impl_t));
    if (table == NULL){
        fprintf(stderr, "Error: Memory allocation failed for table\n");
        exit(1);
    }

    table->capacity = initial_capacity;
    table->size = 0;
    table->pairs = malloc(table->capacity * sizeof(pair_t));

    if (table->pairs == NULL){
        fprintf(stderr, "Error: Memory allocation failed for table pairs\n");
        free(table);
        exit(1);
    }

    return table;
}

void table_free(table_t table){
    for (int i = 0; i < table->size; i++){
        free(table->pairs[i].key);
    }
    free(table->pairs);
    free(table);
}

void table_print(table_t table){
    printf("\n--- Printing contents of table (Size: %d /Capacity: %d) ---\n", table->size, table->capacity);
    for(int i = 0; i < table->size; i++){
        pair_print(table->pairs[i]);
    }
}

void table_set(table_t table, const char *key, int value){
    // Existing key
    for (int i = 0; i < table->size; i++){
        if (strcmp(table->pairs[i].key, key) == 0){
            table->pairs[i].value = value;
            return;
        }
    }
    // New key
    if (table->size == table->capacity){
        // Reallocate memory
        table->capacity *= 2;
        pair_t *new_table = realloc(table->pairs, table->capacity * sizeof(pair_t));
        if (new_table == NULL){
            fprintf(stderr, "Error: Out of memory\n");
            table_free(table);
            exit(1);
        }
        table->pairs = new_table;
    }
    // Add pair to table
    table->pairs[table->size] = pair_create(key, value);
    table->size++;
}

int table_get(const table_t table, const char *key, int *value){
    for (int i = 0; i < table->size; i++){
        if (strcmp(table->pairs[i].key, key) == 0){
            if (value != NULL){
                *value = table->pairs[i].value;
            }
            return 1; // Key esists
        }
    }
    return 0; // Key does not exist
}

// Pair functions
static pair_t pair_create(const char *key, int value){
    pair_t pair;
    pair.key = strdup(key);
    pair.value = value;
    return pair;
}

static void pair_print(pair_t pair){
    printf("%s | %d\n", pair.key, pair.value);
}