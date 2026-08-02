/**
 * @file dynamic_array.c
 * @brief Implementation for dynamic_array.h
 */

#include "../include/dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct array_impl
 * @brief Internal implementation of the dynamic array.
 */
typedef struct array_impl {
    int capacity; /**< Spaces currently allocated for the dynamic array. */
    int size;     /**< Spaces currently occupied by the dynamic array. */
    void **data;  /**< Core array of data. */
} array_impl_t;

array_t array_create(int initial_capacity) {
    if (initial_capacity <= 0) {
        fprintf(stderr, "Error: array_create received initial_capacity less than 1\n");
        exit(1);
    }

    array_t array = malloc(sizeof(array_impl_t));

    if (array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for array\n");
        exit(1);
    }

    array->capacity = initial_capacity;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(void *));

    return array;
}

array_t array_dupe(array_t array) {
    array_t copy = array_create(array->capacity);

    for (int i = 0; i < array->size; i++) {
        array_append(copy, array->data[i]);
    }

    return copy;
}

void array_append(array_t array, void *value) {
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->capacity * sizeof(void *));
    }

    if (array->data == NULL) {
        fprintf(stderr, "Error: array_append failed to allocate more memory for new entry\n");
        exit(1);
    }

    array->data[array->size] = value;
    array->size++;
}

void *array_pop(array_t array) {
    if (array->size < 1)
        return NULL;

    void *value = array->data[array->size - 1];
    array->size -= 1;

    return value;
}

void array_free(array_t array) {
    free(array->data);
    free(array);
}

void *array_set(array_t array, void *value, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function array_set out of bounds\n");
        exit(1);
    }

    void *stale_value = array->data[index];
    array->data[index] = value;
    return stale_value;
}

const void *array_get(const array_t array, int index) {
    if (index < 0 || index >= array->size) {
        fprintf(stderr, "Error: function array_get out of bounds\n");
        exit(1);
    }

    return array->data[index];
}

int array_get_size(const array_t array) { return array->size; }

