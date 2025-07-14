#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>

void return_scalar_udf(
    INTEGER *rownum,
    VARCHAR_LATIN *textval,
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257]
)
{
    switch (*rownum) {
        case 1: strcpy(textval, "Row One"); break;
        case 2: strcpy(textval, "Row Two"); break;
        case 3: strcpy(textval, "Row Three"); break;
        default: strcpy(textval, "Unknown Row"); break;
    }
    strcpy(sqlstate, "00000"); // success
}
