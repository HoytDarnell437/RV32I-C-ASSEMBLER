#include <stdio.h>
#include <string.h>

/* Helper Functions */
/**
 * @brief Convert strings into their ascii values.
 * @param str String to parse the value of.
 */
static int parse_value(const char *str);

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

char *array[] = {
    "'a'",  // 97
    "56",   // 56
    "'5'",  // 53
    "0x50", // 80
    "0x0",  // 0
    "",    // 0
    NULL};

int main() {
    int i = -1;
    while (array[++i] != NULL) {
        printf("String: %s | Value: %d\n", array[i], parse_value(array[i]));
    }
}

static int parse_value(const char *str) {
    if (strlen(str) < 1) {
        fprintf(stderr, "Error: Impropper input to parse_value function: NULL or Empty str\n");
        return (0);
    }

    if (str[0] == '\'' && str[strlen(str) - 1] == '\'') {
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
