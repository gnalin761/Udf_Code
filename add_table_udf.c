#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>

void add_table_udf(
    INTEGER *a,
    INTEGER *b,
    INTEGER *sum_result,
    int *indicator_a,
    int *indicator_b,
    int *indicator_sum_result,
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257])
{
    if (*indicator_a == -1 || *indicator_b == -1) {
        *indicator_sum_result = -1; // return NULL if any input is NULL
    } else {
        *sum_result = *a + *b;
        *indicator_sum_result = 0;
    }

    strcpy(sqlstate, "00000");  // success
}
