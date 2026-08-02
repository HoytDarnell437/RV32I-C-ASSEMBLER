/**
 * @file dynamic_array.h
 * @author Hoyt Darnell
 * @date 2026-08-01
 * @brief Dynamically allocated array structs and related functions.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>

struct array_impl;

/**
 * @typedef array_t
 * @brief Opaque handle to a dynamic array.
 */
typedef struct array_impl *array_t;

/**
 * @brief Creates a array_t type.
 * @param initial_capacity Initial capacity of created array.
 * @return Returns a pointer to the created dynamic array.
 */
array_t array_create(int initial_capacity);

/**
 * @brief Creates a copy of a given array_t.
 * @param array Array to create a copy of.
 * @return Returns a pointer to the copy of array.
 */
array_t array_dupe(array_t array);

/**
 * @brief Appends a value to a given array_t.
 * @param array Array to append to.
 * @param[in] value Value to append onto array.
 * @note Value is not directly appended a deep copy is instead appended.
 */
void array_append(array_t array, void *value);

/**
 * @brief Pops the last value off of an array.
 * @param array Array to remove value from.
 * @return Returns the popped value.
 */
void *array_pop(array_t array);

/**
 * @brief Frees the array.
 * @note It is the user's responsibility to free data entries.
 * @param array Array to free.
 */
void array_free(array_t array);

/**
 * @brief Sets the value of array at index to value.
 * @param array Array to set.
 * @param value Value to set.
 * @param index Index of array to set.
 * @return Returns pointer to stale data value which user is now responsible for.
 */
void *array_set(array_t array, void *value, int index);

/**
 * @brief Retrieves the value of array at index.
 * @param array Array to retrieve from.
 * @param index Index of array to retrieve from.
 * @return Pointer to value requested.
 */
const void *array_get(const array_t array, int index);

/**
 * @brief Retrieves the size of array.
 * @param[in] array Array to retieve size from.
 * @return Size of array.
 */
int array_get_size(const array_t array);

#endif // DYNAMIC_ARRAY_H
