/**
 * @file register.h
 * @author Hoyt Darnell
 * @date 2026-06-27
 * @brief This library provides the functionality of lookup table for register values
 */

#ifndef REGISTER_H
#define REGISTER_H

/**
 * @brief Searches the table and returns the value associated with the key.
 * @param key String associated with the register (x1, gp, ...).
 * @return Returns the integer value associated with the register.
 */
int register_lookup(const char *key);

#endif // REGISTER_H