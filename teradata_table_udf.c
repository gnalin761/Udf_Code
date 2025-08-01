#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlda.h>
#include <sqltypes_td.h>

// This is the context structure that holds the state of the UDF instance.
// It is allocated in the TABLE_OPEN phase and freed in the TABLE_CLOSE phase.
typedef struct {
    DBCHandle   dbch;       // Database connection handle
    STMTHandle  stmt;       // Statement handle
    SQL_INTEGER output1_data; // Buffer for INTEGER output
    SQL_Varchar output2_data[50]; // Buffer for VARCHAR output
} UDFTableContext;

// This is the main UDF function that Teradata calls.
// It's a phased UDF, so its behavior depends on the value of tc->call_type.
void my_phased_table_udf(TableContext* tc,
                         SQL_INTEGER *output1,  // Output parameter for the first column
                         SQL_Varchar *output2,  // Output parameter for the second column
                         Indicator* output_ind1,
                         Indicator* output_ind2) {
    
    // A pointer to our context struct, which is stored in the TableContext.
    UDFTableContext* context = (UDFTableContext*)tc->context;

    // The logic inside a Table UDF is broken down by the call_type.
    switch (tc->call_type) {

        case TABLE_OPEN: {
            // This block runs only once when the UDF is first called.
            // It's used for initialization and setup.

            // --- 1. Define Teradata connection parameters ---
            // IMPORTANT: Replace these with your actual Teradata server details.
            const char *td_hostname = "jdbc:teradata://localhost/DATABASE=,ndw_tempDBS_PORT=1025";
            const char *username    = "gnalin761";
            const char *password    = "Ramashray240592@2025";

            // --- 2. Define the hard-coded SQL query ---
            // This query must return a result set that matches the UDF's output columns.
            const char *sql_query = "SELECT CAST(c1 AS INTEGER) AS id, c2 FROM (VALUES (1, 'Hello UDF!'), (2, 'World!'), (3, 'How are you?')) AS T(c1, c2)";

            // --- 3. Allocate UDF context and store it in TableContext ---
            context = (UDFTableContext*)calloc(1, sizeof(UDFTableContext));
            if (!context) {
                tc->error_message = "Memory allocation failed.";
                return;
            }
            tc->context = (void*)context;

            // --- 4. CLIv2 function calls for connection and execution ---
            if (DB_INIT() != CLI_SUCCESS) {
                tc->error_message = "Error: CLIv2 initialization failed.";
                return;
            }

            if (DB_CONNECT(&context->dbch, 100, (char *)td_hostname, (char *)username, (char *)password, 0) != CLI_SUCCESS) {
                tc->error_message = "Error: Connection failed.";
                DB_FINI();
                return;
            }

            if (DB_STMT(&context->dbch, &context->stmt) != CLI_SUCCESS) {
                tc->error_message = "Error: Statement allocation failed.";
                DB_DISCONNECT(&context->dbch);
                DB_FINI();
                return;
            }

            if (DB_PREPARE(context->stmt, (char *)sql_query, strlen(sql_query)) != CLI_SUCCESS) {
                tc->error_message = "Error: SQL statement preparation failed.";
                return;
            }

            if (DB_EXECUTE(context->stmt) != CLI_SUCCESS) {
                tc->error_message = "Error: SQL statement execution failed.";
                return;
            }

            // Bind the result columns to the buffers in our context.
            if (DB_BIND_COL(context->stmt, 1, DB_INT_TYPE, &context->output1_data, sizeof(SQL_INTEGER), 0) != CLI_SUCCESS) {
                tc->error_message = "Error: Binding result column 1 failed.";
                return;
            }
            if (DB_BIND_COL(context->stmt, 2, DB_VCHAR_TYPE, &context->output2_data, sizeof(context->output2_data), 0) != CLI_SUCCESS) {
                tc->error_message = "Error: Binding result column 2 failed.";
                return;
            }
            break;
        }

        case TABLE_RETURN_ROWS: {
            // This block runs repeatedly to fetch and return a single row.
            
            // Attempt to fetch the next row from the result set.
            if (DB_FETCH(context->stmt) == CLI_SUCCESS) {
                // Fetch was successful, so we have a new row.
                // Copy the data from our context buffers to the UDF's output parameters.
                *output1 = context->output1_data;
                // For VARCHAR, we need to copy the length and the data.
                int len = *((short *)context->output2_data);
                if (len > 0) {
                    strncpy((char *)(output2 + sizeof(short)), (char *)(context->output2_data + sizeof(short)), len);
                    *((short *)output2) = len;
                } else {
                    *((short *)output2) = 0;
                }

                // Set indicators to 0 for non-null values.
                *output_ind1 = 0;
                *output_ind2 = 0;

                tc->return_rows = 1; // Signal that one row is ready to be returned.
            } else {
                // DB_FETCH failed, which means there are no more rows.
                tc->return_rows = 0; // Signal that the UDF is finished.
            }
            break;
        }

        case TABLE_CLOSE: {
            // This block runs only once when the UDF is finished.
            // Clean up all resources.
            if (context) {
                DB_FREE_STMT(context->stmt);
                DB_DISCONNECT(&context->dbch);
                DB_FINI();
                free(context);
                tc->context = NULL;
            }
            break;
        }

        default:
            // Handle any other unexpected call types.
            strcpy(tc->sqlstate, "U0005");
            strcpy(tc->error_message, "Table function being called in unsupported mode.");
            return;
    }
}
