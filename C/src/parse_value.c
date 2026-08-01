/**
 * @file parse_value.c
 * @brief Implementation for parse_value.h
 */

#include <stdio.h>
#include <string.h>
#include "../include/parse_value.h"

/**
 * @brief Convert strings of decimal numbers into integers.
 * @param str String to convert.
 */
static int dec_str_int(const char *str);

/**
 * @brief Convert strings of hexadecimal numbers into integers.
 * @param str String to convert.
 */
static int hex_str_int(const char *str);

int parse_value(const char *str) {
    if (strlen(str) < 1) {
        fprintf(stderr, "Error: Impropper input to parse_value function: NULL or Empty str\n");
        return (0);
    }

    if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
        if (str[1] == '\\') {
            switch (str[2]) {
            case '0':
                return 0;
            case 'n':
                return 10;
            case 't':
                return 9;
            case '\\':
                return 92;
            case '\'':
                return 39;
            }
        }

        return str[1];
    }

    int value = 0;

    if (str[0] == '0' && str[1] == 'x') {
        if (str[2] == '\0') {
            fprintf(stderr, "Error: Empty hex constant given to parse_value function \"0x\"\n");
        }

        value = hex_str_int(str);

    } else {
        value = dec_str_int(str);
    }

    return value;
}

static int dec_str_int(const char *str) {
    int value = 0;
    int negative = 0;
    int i = 0;

    if (str[0] == '-') {
        negative = 1;
        i++;
    }

    do {
        value *= 10;
        value += ((int)str[i] - 48);
    } while (str[++i] != '\0');

    if (negative) {
        value = -value;
    }

    return value;
}

static int hex_str_int(const char *str) {
    int value = 0;
    int i = 2;

    do {
        value *= 16;
        if (str[i] < 58) {
            value += ((int)str[i] - 48);
        } else {
            value += ((int)str[i] - 87);
        }
    } while (str[++i] != '\0');

    return value;
}
