#include <stdio.h>
#include <string.h>

// Simulated UDF function
void add_two_numbers_udf(
    int *a,
    int *b,
    int *result,
    int *indicator_a,
    int *indicator_b,
    int *indicator_result,
    char sqlstate[6],
    char extname[129],
    char specific_name[129],
    char error_message[257])
{
    if (*indicator_a == -1 || *indicator_b == -1) {
        strcpy(sqlstate, "22004"); // NULL input
        return;
    }

    *result = *a + *b;
    *indicator_result = 0; // Not null
}

// Main function for testing
int main() {
    int a = 10;
    int b = 15;
    int result;
    int indicator_a = 0;
    int indicator_b = 0;
    int indicator_result;
    char sqlstate[6] = "";
    char extname[129] = "";
    char specific_name[129] = "";
    char error_message[257] = "";

    add_two_numbers_udf(&a, &b, &result, &indicator_a, &indicator_b, &indicator_result, sqlstate, extname, specific_name, error_message);

    if (strlen(sqlstate) > 0) {
        printf("SQLSTATE: %s\n", sqlstate);
    } else {
        printf("Result: %d\n", result);
    }

    return 0;
}

