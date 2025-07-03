#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#include <string.h>

void return_table_udf(
    INTEGER *rownum,
    VARCHAR_LATIN *textval,
    char sqlstate[6],
    SQL_TEXT extname[129],
    SQL_TEXT specific_name[129],
    SQL_TEXT error_message[257])
{
    static int counter = 0;

    if (counter >= 3) {
        // No more rows
        counter = 0;
        strcpy(sqlstate, "02000");  // end of data
        return;
    }

    // Populate output values
    *rownum = counter + 1;
    switch (counter) {
        case 0: strcpy(textval, "Row One"); break;
        case 1: strcpy(textval, "Row Two"); break;
        case 2: strcpy(textval, "Row Three"); break;
    }

    counter++;
    strcpy(sqlstate, "00000");  // success
}

