#define SQL_TEXT Latin_Text
#include <sqltypes_td.h>
#define IsNull -1
#define IsNotNull 0
#define NUL '\0'
#define ASCII_ANYTYPES 2



void ascii(void      * inputStr,
           void      * result,
           int       * inputStrIsNull,
           int       * resultIsNull,
           char        sqlstate[6],
           SQL_TEXT    extname[129],
           SQL_TEXT    specific_name[129],
           SQL_TEXT    error_message[257])
{
   int numAnytypeParams;
   int buffersize;
   int returnValue;

   /* Returns NULL given NULL */
   if (*inputStrIsNull == IsNull)
   {
      *resultIsNull = IsNull;
      return;
   }

   buffersize = ASCII_ANYTYPES * sizeof(anytype_param_info_t);
   anytype_param_info_t * inputInfo = FNC_malloc(buffersize);
   FNC_GetAnytypeParamInfo( buffersize, &numAnytypeParams,inputInfo);

   if ( numAnytypeParams >= 2
      && inputInfo[0].paramIndex == 1
      && ( inputInfo[0].datatype == VARCHAR_DT
         || inputInfo[0].datatype == CHAR_DT)
      )
   {
      /* If the input is latin. */
      if(inputInfo[0].charset == LATIN_CT)
      {
         VARCHAR_LATIN * latinInputStr = (VARCHAR_LATIN *) inputStr;
         /* If the string is empty */
         if (*latinInputStr == NUL)
         {
            *resultIsNull = IsNull;
         }
         else
         {
            *resultIsNull = IsNotNull;
            returnValue = (INTEGER)(latinInputStr[0]);
         }
      }

      else if(inputInfo[0].charset == UNICODE_CT)
      {
         VARCHAR_UNICODE * unicodeInputStr = (VARCHAR_UNICODE *) inputStr;
      
         if (*unicodeInputStr == NUL)
         {
            *resultIsNull = IsNull;
         }
         else
         {
            *resultIsNull = IsNotNull;
            returnValue = (INTEGER)(unicodeInputStr[0]);
         }
      }
      
      If (inputInfo[1].paramIndex == -1 && inputInfo[1].datatype == BYTEINT_DT)
      {
         * (BYTEINT *) result = (BYTEINT) returnValue;
      }
      else if ( inputInfo[1].paramIndex == -1
         && inputInfo[1].datatype == SMALLINT_DT)
      {
         * (SMALLINT *) result = (SMALLINT) returnValue;
      }
      else if ( inputInfo[1].paramIndex == -1
         && inputInfo[1].datatype == INTEGER_DT)
      {
         * (INTEGER *) result = returnValue;
      }
      else 
      {
         strcpy(sqlstate, "22023");
         strcpy((char *) error_message, "Invalid result type.") ;
      }
   }
   else
   {
      strcpy(sqlstate, "22023");
      strcpy((char *) error_message, "Invalid input type.") ;
   }
 
   FNC_free(inputInfo);
}
