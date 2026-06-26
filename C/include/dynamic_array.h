// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Last Updated: 06/24/2026
// Usage: This library provides the functionality of a dynamically allocated array of strings

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


// Array type implementation
struct array_impl;
typedef struct array_impl* array_t;

//Master array type implementation
struct master_array_impl;
typedef struct master_array_impl* master_array_t;

// Public API functions
array_t array_create(int initial_capacity); // Create and initialize array variable 
array_t array_dupe(array_t array); // Returns deep copy of array
void array_append(array_t array, const char *str); // Appends a string to the array
char *array_pop(array_t array); // Pops off the last value in the array
void array_free(array_t array); // Deallocates the memory used by the array
void array_print(array_t array); // Prints the contents of the array to terminal

master_array_t master_array_create(int initial_capacity); // Create and initialize master array variable
void master_array_append(master_array_t master, array_t sub_array); // Appends array to the master_array
void master_array_free(master_array_t master); // Deallocates the memory used by the master_array
void master_array_print(master_array_t master); // Prints the contents of the master array to terminal

void array_set(array_t array, const char *str, int index); // Changes the string at index of the array to the one provided
char *array_get(const array_t array, int index); // Retrieves the string at the index of the array
int array_get_size(const array_t array); // Retrieves the size of the array

#endif // DYNAMIC_ARRAY_H