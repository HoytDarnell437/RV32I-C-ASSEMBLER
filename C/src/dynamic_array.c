/**
 * @file dynamic_array.c
 * @brief Implementation for dynamic_array.h
 */

#include "../include/dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct char_array_impl
 * @brief Internal implementation of the dynamic array of strings.
 */
typedef struct char_array_impl {
    int capacity; /**< Spaces currently allocated for the dynamic array. */
    int size;     /**< Spaces currently occupied by the dynamic array. */
    char **data;  /**< Core array of strings. */
} char_array_impl_t;

/**
 * @struct int_array_impl
 * @brief Internal implementation of the dynamic array of integers.
 */
typedef struct int_array_impl {
    int capacity; /**< Spaces currently allocated for the dynamic array. */
    int size;     /**< Spaces currently occupied by the dynamic array. */
    int *data;    /**< Core array of integers. */
} int_array_impl_t;

/**
 * @struct master_array_impl
 * @brief Internal implementation of the master array.
 */
typedef struct master_array_impl {
    int capacity;         /**< Spaces currently allocated for the master array. */
    int size;             /**< Spaces currently occupied by the master array. */
    char_array_t *arrays; /**< Core array of the master array. */
} master_array_impl_t;

char_array_t char_array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: char_array_create received initial_capacity less than 1\n");
        exit(1);
    }

    char_array_t array = malloc(sizeof(char_array_impl_t));

    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for char_array\n");
        exit(1);
    }

    array->capacity = initial_capacity;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(char *));

    return array;
}

char_array_t char_array_dupe(char_array_t array) {
    char_array_t copy = char_array_create(array->capacity);

    for (int i = 0; i < array->size; i++) {
        char_array_append(copy, array->data[i]);
    }

    return copy;
}

void char_array_append(char_array_t array, const char *str) {
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

char *char_array_pop(char_array_t array) {
    if (array->size < 1)
        return NULL;

    char *copy = array->data[array->size - 1];
    array->data[array->size - 1] = NULL;
    array->size -= 1;

    return copy;
}

void char_array_free(char_array_t array) {
    for (int i = 0; i < array->size; i++) {
        free(array->data[i]);
    }

    free(array->data);
    free(array);
}

void char_array_print(const char_array_t array, FILE *file) {
    fprintf(file, "\n--- Printing contents of dynamic string array (Size: %d  / Capacity: %d) ---\n", array->size, array->capacity);

    for (int i = 0; i < array->size; i++) {
        fprintf(file, "%s\n", array->data[i] ? array->data[i] : "(NULL)");
    }
}

void char_array_set(char_array_t array, const char *str, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function char_array_set out of bounds\n");
        exit(1);
    }

    free(array->data[index]);

    if (str != NULL) {
        array->data[index] = strdup(str);
    } else {
        array->data[index] = NULL;
    }
}

char *char_array_get(const char_array_t array, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function char_array_get out of bounds\n");
        exit(1);
    }

    return array->data[index];
}

int char_array_get_size(const char_array_t array) { return array->size; }

int_array_t int_array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: int_array_create received initial_capacity less than 1\n");
        exit(1);
    }

    int_array_t array = malloc(sizeof(int_array_impl_t));

    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for int_array\n");
        exit(1);
    }

    array->capacity = initial_capacity;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(int));

    return array;
}

int_array_t int_array_dupe(int_array_t array) {
    int_array_t copy = int_array_create(array->capacity);

    for (int i = 0; i < array->size; i++) {
        int_array_append(copy, array->data[i]);
    }

    return copy;
}

void int_array_append(int_array_t array, int num) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(int));
    }

    array->size++;
}

void int_array_free(int_array_t array) {
    free(array->data);
    free(array);
}

void int_array_print(const int_array_t array, FILE *file) {
    fprintf(file, "\n--- Printing contents of dynamic integer array (Size: %d  / Capacity: %d) ---\n", array->size, array->capacity);

    for (int i = 0; i < array->size; i += 4) {
        fprintf(file, "%x\n", array->data[i]);
    }
}

void int_array_set(int_array_t array, int num, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function int_array_set out of bounds\n");
        exit(1);
    }

    array->data[index] = num;
}

int int_array_get(const int_array_t array, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function int_array_get out of bounds\n");
        exit(1);
    }

    return array->data[index];
}

int int_array_get_size(const int_array_t array) { return array->size; }

// Master array functions
master_char_array_t master_array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: master_array_create received initial_capacity less than 1\n");
        exit(1);
    }

    master_char_array_t master = malloc(sizeof(master_array_impl_t));

    if (master == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for master array\n");
        exit(1);
    }

    master->capacity = initial_capacity;
    master->size = 0;
    master->arrays = malloc(master->capacity * sizeof(char_array_t));

    return master;
}

void master_array_append(master_char_array_t master, char_array_t sub_array) {
    if (sub_array == NULL) {
        return;
    }

    if (master->size == master->capacity) {
        master->capacity *= 2;
        char_array_t *new_arrays = realloc(master->arrays, master->capacity * sizeof(char_array_t));
        master->arrays = new_arrays;
    }

    master->arrays[master->size] = sub_array;
    master->size++;
}

void master_array_free(master_char_array_t master) {
    for (int i = 0; i < master->size; i++) {
        char_array_free(master->arrays[i]);
    }

    free(master->arrays);
    free(master);
}

void master_array_print(master_char_array_t master, FILE *file) {
    fprintf(file, "\n--- Printing contents of master array (Size: %d  / Capacity: %d) ---\n", master->size, master->capacity);

    for (int i = 0; i < master->size; i++) {
        fprintf(file, "\n--- Sub Array %d ---\n", i);

        for (int j = 0; j < master->arrays[i]->size; j++) {
            fprintf(file, "%s\n", master->arrays[i]->data[j] ? master->arrays[i]->data[j] : "(NULL)");
        }
    }
}

char_array_t master_array_get(const master_char_array_t master, int index) {
    if (index < 0 || index >= master->size) {
        fprintf(stderr, "Error: function master_array_get out of bounds\n");
        exit(1);
    }

    return master->arrays[index];
}

int master_array_get_size(const master_char_array_t master) {
    return master->size;
}
