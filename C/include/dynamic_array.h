// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Last Updated: 06/24/2026
// Usage: This library provides the functionality of a dynamically allocated array of strings

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


// Internal array implementation
struct array_impl;

// Public pointer to array_impl
typedef struct array_impl* array_t;

// Public API functions
array_t array_create(int initial_capacity); // Create and initialize array variable 
void array_push(array_t array, const char *str); // Adds a string to the end of the array
void array_free(array_t array); // Deallocates the memory used by the array
void array_print(array_t array); // Prints the contents of the array to terminal

void array_set(array_t array, const char *str, int index); // Changes the string at index of the array to the one provided
const char *array_get(const array_t array, int index); // Retrieves the string at the index of the array

#endif // DYNAMIC_ARRAY_H