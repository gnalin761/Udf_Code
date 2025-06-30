#include <sqltypes_td.h>
#include <string.h>

#define SQL_TEXT Latin_Text

// UDF function: returns the sum of two integers as a table output
void add_two_numbers(
    INTEGER *a,                // Input 1
    INTEGER *b,                // Input 2
    INTEGER *call_count,       // Row index
    INTEGER *result,           // Output: sum of a + b
    char sqlstate[6]           // SQLSTATE
)
{
    if (*call_count == 0) {
        *result = *a + *b;
    } else {
        *call_count = -1;  // No more rows
    }

    strcpy(sqlstate, "00000"); // Success
}
