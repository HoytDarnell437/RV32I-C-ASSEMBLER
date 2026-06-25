// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Implementation for dynamic_array.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"


// Struct for dynamic array
typedef struct array_impl{
    int capacity;
    int size;
    char **data;
} array_impl_t;

// Array functions
array_t array_create(int initial_capacity){
    if (initial_capacity <= 0){
        fprintf(stderr, "Error: array_create received initial_capacity less than 1\n");
        exit(1);
    }

    array_t array = malloc(sizeof(array_impl_t));

    if (array == NULL){
        fprintf(stderr, "Error: Memory allocation failed for dynamic array\n");
        exit(1);
    }

    array->capacity = initial_capacity;
    array->size = 0;
    array->data = malloc(array->capacity * sizeof(char *));

    if (array->data == NULL){
        fprintf(stderr, "Error: Memory allocation failed for dynamic array data\n");
        free(array);
        exit(1);
    }

    return array;
}

void array_push(array_t array, const char *str){
    if (array->size == array->capacity){
        // Double allocated memory when none is left
        array->capacity *= 2;
        char **new_data = realloc(array->data, array->capacity * sizeof(char *));
        if (new_data == NULL){
            fprintf(stderr, "Error: Out of memory\n");
            exit(1);
        }
        array->data = new_data;
    }
    // Duplicate string into new spot
    if (str != NULL){
        array->data[array->size] = strdup(str);
        if (array->data[array->size] == NULL){
            fprintf(stderr, "Error: Out of memory\n");
            exit(1);
        }
    }
    else {
        array->data[array->size] = NULL;
    }
    array->size++;
}

void array_free(array_t array){
    for (int i = 0; i < array->size; i++){
        free(array->data[i]);
    }
    free(array->data);
    free(array);
}

void array_print(array_t array){
    printf("Printing contents of dynamic array\nCapacity: %d\nSize: %d\nData:\n", array->capacity, array->size);
    for (int i = 0; i < array->size; i++){
        if (array->data[i] == NULL){
            continue;
        }
        printf("%s\n", array->data[i]);
    }
}

void array_set(array_t array, const char *str, int index){
    if (index < 0 || index >= array->size){
        fprintf(stderr, "Error: function array_set out of bounds\n");
        exit(1);
    }
    free(array->data[index]);
    array->data[index] = (str != NULL) ? strdup(str) : NULL;
}

const char *array_get(const array_t array, int index){
    if (index < 0 || index >= array->size){
        fprintf(stderr, "Error: function array_get out of bounds\n");
        exit(1);
    }
    return array->data[index];
}