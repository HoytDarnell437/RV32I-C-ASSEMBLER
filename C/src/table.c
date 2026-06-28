/**
 * @file table.c
 * @brief Implementation for table.h
 */

#include <stdlib.h>
#include <string.h>
#include "../include/table.h"

/**
 * @struct table_impl
 * @brief Internal implementation of the linear lookup table.
 */
typedef struct table_impl {
    int capacity; /**< Spaces currently allocated for the table. */
    int size;  /**< Spaces currently occupied by the table. */
    pair_t *pairs; /**< Core array of pairs. */
} table_impl_t;

static pair_t pair_create(const char *key, int value);
static void pair_print(const pair_t pair, FILE *file);

table_t table_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: table_create received initial_capacity less than 1\n");
        exit(1);
    }

    table_t table = malloc(sizeof(table_impl_t));

    table->capacity = initial_capacity;
    table->size = 0;
    table->pairs = malloc(table->capacity * sizeof(pair_t));

    return table;
}

void table_free(table_t table) {
    for (int i = 0; i < table->size; i++) {
        free(table->pairs[i].key);
    }

    free(table->pairs);
    free(table);
}

void table_print(const table_t table, FILE *file) {
    fprintf(file, "\n--- Printing contents of table (Size: %d /Capacity: %d) ---\n", table->size, table->capacity);

    for(int i = 0; i < table->size; i++) {
        pair_print(table->pairs[i], file);
    }
}

void table_set(table_t table, const char *key, int value) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->pairs[i].key, key) == 0) {
            table->pairs[i].value = value;
            return;
        }
    }

    if (table->size == table->capacity) {
        table->capacity *= 2;
        table->pairs = realloc(table->pairs, table->capacity * sizeof(pair_t));
    }

    table->pairs[table->size] = pair_create(key, value);
    table->size++;
}

int table_get(const table_t table, const char *key, int *value) {
    for (int i = 0; i < table->size; i++) {
        if (strcmp(table->pairs[i].key, key) == 0) {
            if (value != NULL) {
                *value = table->pairs[i].value;
            }

            return 1;
        }
    }

    return 0;
}

static pair_t pair_create(const char *key, int value){
    pair_t pair;

    pair.key = strdup(key);
    pair.value = value;

    return pair;
}

static void pair_print(const pair_t pair, FILE *file){
    fprintf(file, "%s | %d\n", pair.key, pair.value);
}