#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>

void new_scalar_udf(
    VARCHAR_LATIN *textval,
    INTEGER *rownum,
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257]
)
{
    strcpy(sqlstate, "00000");
    error_message[0] = '\0';

    if (rownum == NULL) {
        strcpy(sqlstate, "22002");
        strcpy(error_message, "Input 'rownum' cannot be NULL for this function.");
        return;
    }

    switch (*rownum) {
        case 1:
            strcpy(textval, "Row One");
            break;
        case 2:
            strcpy(textval, "Row Two");
            break;
        case 3:
            strcpy(textval, "Row Three");
            break;
        default:
            strcpy(textval, "Unknown Row");
            break;
    }
}
