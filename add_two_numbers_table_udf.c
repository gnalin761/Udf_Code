#include "sqltypes_td.h"
#include <string.h>

void add_two_numbers_table_udf(
    FNC_Phase phase,
    INT *a,
    INT *b,
    INT *out_a,
    INT *out_b,
    INT *out_sum,
    SQL_TEXT sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257])
{
    switch (phase)
    {
    case FNC_INIT:
        break;

    case FNC_ROW:
        if (a == NULL || b == NULL) {
            strncpy(sqlstate, "22004", 6); // NULL input
            return;
        }

        *out_a = *a;
        *out_b = *b;
        *out_sum = *a + *b;

        FNC_TblOptOut();  // Send this result row
        break;

    case FNC_END:
        break;
    }
}
