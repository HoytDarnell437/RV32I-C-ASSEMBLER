/**
 * @file dynamic_array.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief Dynamically allocated string array and 2D master array interfaces
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>

struct char_array_impl;

/**
 * @typedef char_array_t
 * @brief Opaque handle to a dynamic array of character arrays instance.
 */
typedef struct char_array_impl* char_array_t;

struct master_array_impl;

/**
 * @typedef master_char_array_t
 * @brief Opaque handle to a master array instance.
 */
typedef struct master_array_impl* master_char_array_t;

/**
 * @brief Creates an array_t type.
 * @param initial_capacity Initial capacity of created array.
 * @return Returns a pointer to the created dynamic array.
 */
char_array_t char_array_create(int initial_capacity);

/**
 * @brief Creates a deep copy of a given char_array_t.
 * @param array Array to create a deep copy of.
 * @return Returns a pointer to the deep copy of array.
 */
char_array_t char_array_dupe(char_array_t array);

/**
 * @brief Appends a str to a given char_array_t.
 * @param array Array to append to.
 * @param[in] str String to append onto array.
 * @note String is not directly appended a deep copy is instead appended.
 */
void char_array_append(char_array_t array, const char *str);

/** 
 * @brief Pops the last str off of an array.
 * @param array Array to remove str from.
 * @return Returns the popped string.
 */
char *char_array_pop(char_array_t array);

/** 
 * @brief Frees all elements in the given array.
 * @param array Array to free.
 */
void char_array_free(char_array_t array);

/**
 * @brief Prints the contents of array.
 * @param array Array to print.
 * @param file File to print to.
 */
void char_array_print(char_array_t array, FILE *file);

/**
 * @brief Creates a master_char_array_t type.
 * @param initial_capacity Initial capacity of created master array.
 * @return Returns a pointer to the created master array.
 */
master_char_array_t master_array_create(int initial_capacity);

/**
 * @brief Appends an char_array_t to a given master_char_array_t.
 * @param master Master array to append to.
 * @param sub_array Array to append onto master array.
 */
void master_array_append(master_char_array_t master, char_array_t sub_array);

/**
 * @brief Frees all elements in the given master array.
 * @param master Master array to free.
 */
void master_array_free(master_char_array_t master);

/**
 * @brief Prints the contents of master array.
 * @param master Master array to print.
 * @param file File to print to.
 */
void master_array_print(master_char_array_t master, FILE *file);

/**
 * @brief Sets the value of array at index to str.
 * @param array Array to set.
 * @param str String to set.
 * @param index Index of array to set.
 */
void char_array_set(char_array_t array, const char *str, int index);

/**
 * @brief Retrieves the value of array at index.
 * @param array Array to retrieve from.
 * @param index Index of array to retrieve from.
 * @return Pointer to string requested.
 */
char *char_array_get(const char_array_t array, int index);

/**
 * @brief Retrieves the size of array.
 * @param[in] array Array to retieve size from. 
 * @return Size of array.
 */
int char_array_get_size(const char_array_t array);

/**
 * @brief Retrieves the value of array at index.
 * @param master Master array to retrieve from.
 * @param index Index of master array to retrieve from.
 * @return Requested array.
 */
char_array_t master_array_get(const master_char_array_t master, int index);

/**
 * @brief Retrieves the size of master array.
 * @param[in] master Master array to retieve size from. 
 * @return Size of master array.
 */
int master_array_get_size(const master_char_array_t master);

#endif // DYNAMIC_ARRAY_H
