// Engineer: Hoyt Darnell
// Institution: Georgia Institute of Technology
// Last Updated: 06/25/2026
// Usage: This library provides the functionality of lookup table for register values

#ifndef REGISTER_H
#define REGISTER_H
#include "../include/table.h"


// Public API functions
const pair_t *register_lookup(const char *key);

#endif // REGISTER_H