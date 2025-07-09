#include <string.h>
#include <stdio.h>

void reverse_string_udf(const char *input, char *output, int *indicator_input, int *indicator_output, char sqlstate[6], const char function_name[], const char specific_name[], const char error_message[]) {
    if (*indicator_input == -1) {
        *indicator_output = -1;  // NULL input, return NULL
        return;
    }

    size_t len = strlen(input);
    for (size_t i = 0; i < len; ++i) {
        output[i] = input[len - i - 1];
    }
    output[len] = '\0';  // Null-terminate the output string
    *indicator_output = 0;  // Output is not NULL
}


