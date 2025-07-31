#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>

void get_row_description_udf(
    char          *result_data,
    int            result_len,
    INTEGER       *rownum,
    char           sqlstate[6],
    SQL_FLOAT_T   *scratchpad,
    SQL_INT_T     *calltype,
    SQL_BOOLEAN_T *isnull,
    SQL_TEXT       extname[129],
    SQL_TEXT       specific_name[129],
    SQL_TEXT       error_message[257],
    SQL_TEXT       udf_name[129],
    SQL_TEXT       udf_specific_name[129],
    SQL_TEXT       udf_schema_name[129],
    SQL_TEXT       udf_catalog_name[129],
    SQL_TEXT       udf_database_name[129]
)
{
    strcpy(sqlstate, "00000");
    error_message[0] = '\0';

    if (isnull[1]) {
        strcpy(sqlstate, "22002");
        strncpy(error_message, "Input 'rownum' cannot be NULL for this function.", sizeof(error_message) - 1);
        error_message[sizeof(error_message) - 1] = '\0';
        isnull[0] = 1;
        return;
    }

    switch (*rownum) {
        case 1:
            strncpy(result_data, "Row One", result_len - 1);
            break;
        case 2:
            strncpy(result_data, "Row Two", result_len - 1);
            break;
        case 3:
            strncpy(result_data, "Row Three", result_len - 1);
            break;
        default:
            strncpy(result_data, "Unknown Row", result_len - 1);
            break;
    }

    result_data[result_len - 1] = '\0';
    isnull[0] = 0;
}
