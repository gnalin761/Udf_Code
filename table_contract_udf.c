#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>


// Contract Function
void return_table_udf_contract(
    SQL_TEXT result_name[128],
    SQL_TEXT result_type[128],
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257]
) {
    strcpy(result_name, "ID,Name");
    strcpy(result_type, "INTEGER,VARCHAR(20)");
    strcpy(sqlstate, "00000");
}

// Table Function
void return_table_udf(
    INTEGER *rownum,
    INTEGER *id,
    VARCHAR_LATIN *name,
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257],
    int *result_code
) {
    if (*rownum > 3) {
        *result_code = 1; // No more rows
        return;
    }

    *id = *rownum;

    switch (*rownum) {
        case 1: strcpy(name, "Row One"); break;
        case 2: strcpy(name, "Row Two"); break;
        case 3: strcpy(name, "Row Three"); break;
    }

    (*rownum)++;
    *result_code = 0;
    strcpy(sqlstate, "00000");
}


