/**
 * @file parse_value.h
 * @author Hoyt Darnell
 * @date 2026-07-31
 * @brief This library provides the functionality of turning strings into integers "20" => 20, "xa" => 10
 */

#ifndef PARSE_VALUE_H
#define PARSE_VALUE_H

/**
 * @brief Converts strings representing characters into their ascii values, hexadecimal strings into integer value, and decimal strings into integer value.
 * @param str String to parse the value of.
 */
int parse_value(const char *str);

#endif // PARSE_VALUE_H
