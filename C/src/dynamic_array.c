/**
 * @file dynamic_array.c
 * @brief Implementation for dynamic_array.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dynamic_array.h"

/**
 * @struct array_impl
 * @brief Internal implementation of the dynamic array.
 */
typedef struct array_impl {
    int capacity; /**< Spaces currently allocated for the dynamic array. */
    int size; /**< Spaces currently occupied by the dynamic array. */
    char **data; /**< Core array of the dynamic array. */
} array_impl_t;

/**
 * @struct master_array_impl
 * @brief Internal implementation of the master array.
 */
typedef struct master_array_impl {
    int capacity; /**< Spaces currently allocated for the master array. */
    int size; /**< Spaces currently occupied by the master array. */
    array_t *arrays; /**< Core array of the master array. */
} master_array_impl_t;

array_t array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: array_create received initial_capacity less than 1\n");
        exit(1);
    }

    array_t array = malloc(sizeof(array_impl_t));

    array->capacity = initial_capacity;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(char *));

    return array;
}

array_t array_dupe(array_t array) {
    array_t copy = array_create(array->capacity);

    for (int i = 0; i < array->size; i++) {
        array_append(copy, array->data[i]);
    }

    return copy;
}

void array_append(array_t array, const char *str) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(char *));
    }

    if (str != NULL) {
        array->data[array->size] = strdup(str);
    } else {
        array->data[array->size] = NULL;
    }

    array->size++;
}

char *array_pop(array_t array) {
    if(array->size < 1) return NULL;

    char *copy = array->data[array->size - 1];
    array->data[array->size - 1] = NULL;
    array->size -= 1;

    return copy;
}

void array_free(array_t array) {
    for (int i = 0; i < array->size; i++) {
        free(array->data[i]);
    }

    free(array->data);
    free(array);
}

void array_print(const array_t array, FILE *file) {
    fprintf(file, "\n--- Printing contents of dynamic array (Size: %d  / Capacity: %d) ---\n", array->size, array->capacity);

    for (int i = 0; i < array->size; i++) {
        fprintf(file, "%s\n", array->data[i] ? array->data[i] : "(NULL)");
    }
}

void array_set(array_t array, const char *str, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function array_set out of bounds\n");
        exit(1);
    }

    free(array->data[index]);

    if (str != NULL) {
        array->data[index] = strdup(str);
    } else {
        array->data[index] = NULL;
    }
}

char *array_get(const array_t array, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function array_get out of bounds\n");
        exit(1);
    }

    return array->data[index];
}

int array_get_size(const array_t array) {
    return array->size;
}

// Master array functions
master_array_t master_array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: master_array_create received initial_capacity less than 1\n");
        exit(1);
    }

    master_array_t master = malloc(sizeof(master_array_impl_t));

    master->capacity = initial_capacity;
    master->size = 0;
    master->arrays = malloc(master->capacity * sizeof(array_t));

    return master;
}

void master_array_append(master_array_t master, array_t sub_array) {
    if (sub_array == NULL) return;

    if (master->size == master->capacity) {
        master->capacity *= 2;
        array_t *new_arrays = realloc(master->arrays, master->capacity * sizeof(array_t));
        master->arrays = new_arrays;
    }

    master->arrays[master->size] = sub_array;
    master->size++;
}

void master_array_free(master_array_t master) {
    for (int i = 0; i < master->size; i++) {
            array_free(master->arrays[i]);
        }

        free(master->arrays);
        free(master);
}

void master_array_print(master_array_t master, FILE *file) {
    fprintf(file, "\n--- Printing contents of master array (Size: %d  / Capacity: %d) ---\n", master->size, master->capacity);

    for (int i = 0; i < master->size; i++) {
        fprintf(file, "\n--- Sub Array %d ---\n", i);

        for (int j = 0; j < master->arrays[i]->size; j++) {
            fprintf(file, "%s\n", master->arrays[i]->data[j] ? master->arrays[i]->data[j] : "(NULL)");
        }
    }
}

int master_array_get_size(const master_array_t master) {
    return master->size;
}