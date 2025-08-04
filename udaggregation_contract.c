#define  byte unsigned char
#define  boolean int
#define  false 0
#define  true  1
#define  OK    0
#define  SQL_TEXT Latin_Text

#define _CRT_SECURE_NO_DEPRECATE
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqltypes_td.h>

#define FALSE 0
#define TRUE 1

#define MAX_KEY_LEN 100

void  udaggregation_contract (
      INTEGER *result,
      int *indicator_Result,
      char sqlstate[6],
      SQL_TEXT extname[129],
      SQL_TEXT specific_name[129],
      SQL_TEXT error_message[257])
{
   FNC_TblOpColumnDef_t *Output_columns;  
   FNC_TblOpColumnDef_t *Input_columns;   
   char                 *buf;
   int                  colcount;        
   int                  i, j, n, k;
   char                 key[MAX_KEY_LEN]; 
   int                  keylen;           
   Key_info_t           valuesSUM;        
   Key_info_t           valuesAVG;        
   int                  *index;          
                                          

   boolean_t            found;
   char                 colname[FNC_MAXNAMELEN_EON];

 
   FNC_TblOpGetCustomKeyInfoOf("SUM_AGG", &valuesSUM);
   FNC_TblOpGetCustomKeyInfoOf("AVG_AGG", &valuesAVG);


   valuesSUM.values_r = FNC_malloc( sizeof(Values_t) * valuesSUM.numOfVal );
   valuesAVG.values_r = FNC_malloc( sizeof(Values_t) * valuesAVG.numOfVal );
  
   colcount = valuesSUM.numOfVal + valuesAVG.numOfVal;

   FNC_TblOpGetCustomValuesOf(&valuesSUM);

   FNC_TblOpGetCustomValuesOf(&valuesAVG);

   Output_columns = FNC_malloc ( TblOpSIZECOLDEF(colcount) );


   TblOpINITCOLDEF(Output_columns, colcount);


   Input_columns = FNC_malloc ( TblOpSIZECOLDEF( FNC_TblOpGetColCount(0, 'R') ) );


   TblOpINITCOLDEF( Input_columns, FNC_TblOpGetColCount(0, 'R') );
   FNC_TblOpGetColDef(0, 'R', Input_columns);

   index = FNC_malloc( sizeof(int) * colcount );

  

   // Fill in column information for output stream for SUM_AGG
   k = 0;
   found = false;
   for (i = 0; i < valuesSUM.numOfVal; i++)
   {
   
      for (j = 0; j < Input_columns->num_columns; j++)
      {
         if (strncmp(valuesSUM.values_r[i].value,
            Input_columns->column_types[j].column,
            valuesSUM.values_r[i].valueLen) == 0)
         {
               index[k] = j;
               k++;
               found = true;
               break;
         }
      }
      if (! found)
      {
         FNC_TblOpSetError("U0001", "Invalid column name in custom clause.");
         return;
      }
      strcpy(colname, "SUM_");
      strncpy( Output_columns->column_types[i].column,
         strncat(colname, (char *) valuesSUM.values_r[i].value,
        valuesSUM.values_r[i].valueLen),FNC_MAXNAMELEN_EON);
      Output_columns->column_types[i].datatype = REAL_DT;
      Output_columns->column_types[i].bytesize = SIZEOF_FLOAT;
   }

   n = valuesSUM.numOfVal;

   found = false;
   for (i = 0; i < valuesAVG.numOfVal; i++)
   {
 
      for (j = 0; j < Input_columns->num_columns; j++)
      {
         if (strncmp(valuesAVG.values_r[i].value,
            Input_columns->column_types[j].column,
            valuesAVG.values_r[i].valueLen)== 0)
         {
               index[k] = j;
               k++;
               found = true;
               break;
         }
      }
      if (! found)
      {
         FNC_TblOpSetError("U0001", "Invalid column name in custom clause.");
         return;
      }
      strcpy(colname, "AVG_");
      strncpy( Output_columns->column_types[n+i].column,
         strncat(colname, (char *) valuesAVG.values_r[i].value,
            valuesAVG.values_r[i].valueLen), FNC_MAXNAMELEN_EON);
      Output_columns->column_types[n+i].datatype = REAL_DT;
      Output_columns->column_types[n+i].bytesize = SIZEOF_FLOAT;
   }
   // set column definitions for output stream
   FNC_TblOpSetOutputColDef(0, Output_columns);

   // pass indices to table operator in the contract context
   FNC_TblOpSetContractDef(index, sizeof(int) * colcount );

   // release memory
   *result = Output_columns->num_columns;
   FNC_free(Output_columns);
   FNC_free(Input_columns);
   FNC_free(valuesSUM.values_r);
   FNC_free(valuesAVG.values_r);
   FNC_free(index);
}
