/**
 * @file dynamic_array.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Dynamically allocated string array and 2D master array interfaces
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


struct array_impl;
typedef struct array_impl* array_t;

struct master_array_impl;
typedef struct master_array_impl* master_array_t;

/**
 * @brief Creates an array_t type.
 * @param initial_capacity Initial capacity of created array.
 * @return Returns a pointer to the created dynamic array.
 */
array_t array_create(int initial_capacity);

/**
 * @brief Creates a deep copy of a given array_t.
 * @param array Array to create a deep copy of.
 * @return Returns a pointer to the deep copy of array.
 */
array_t array_dupe(array_t array);

/**
 * @brief Appends a str to a given array_t.
 * @param array Array to append to.
 * @param[in] str String to append onto array.
 * @note String is not directly appended a deep copy is instead appended.
 */
void array_append(array_t array, const char *str);

/** 
 * @brief Pops the last str off of an array.
 * @param array Array to remove str from.
 * @return Returns the popped string.
 */
char *array_pop(array_t array);

/** 
 * @brief Frees all elements in the given array.
 * @param array Array to free.
 */
void array_free(array_t array);

/**
 * @brief Prints the contents of array.
 * @param array Array to print.
 * @param file File to print to.
 */
void array_print(array_t array, FILE *file);

/**
 * @brief Creates a master_array_t type.
 * @param initial_capacity Initial capacity of created master array.
 * @return Returns a pointer to the created master array.
 */
master_array_t master_array_create(int initial_capacity);

/**
 * @brief Appends an array_t to a given master_array_t.
 * @param master Master array to append to.
 * @param sub_array Array to append onto master array.
 */
void master_array_append(master_array_t master, array_t sub_array);

/**
 * @brief Frees all elements in the given master array.
 * @param master Master array to free.
 */
void master_array_free(master_array_t master);

/**
 * @brief Prints the contents of master array.
 * @param master Master array to print.
 * @param file File to print to.
 */
void master_array_print(master_array_t master, FILE *file);

/**
 * @brief Sets the value of array at index to str.
 * @param array Array to set.
 * @param str String to set.
 * @param index Index of array to set.
 */
void array_set(array_t array, const char *str, int index);

/**
 * @brief Retrieves the value of array at index.
 * @param array Array to retrieve from.
 * @param index Index of array to retrieve from.
 * @return Pointer to string requested.
 */
char *array_get(const array_t array, int index);

/**
 * @brief Retrieves the size of array.
 * @param[in] array Array to retieve size from. 
 * @return Size of array.
 */
int array_get_size(const array_t array);

#endif // DYNAMIC_ARRAY_H