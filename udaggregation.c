void udaggregation ()
{
   int                  null_ind, length;
   FNC_TblOpHandle_t    *Input_handle;     // input stream handle
   FNC_TblOpHandle_t    *Output_handle;    // output stream handle
   FNC_TblOpColumnDef_t *Input_columns;    // input stream column definitions
   FNC_TblOpColumnDef_t *Output_columns;   // output stream column definitions
   double               *aggregates;       // aggregates computation
   int                  *index;            // columns index in input stream of aggregates
   int                  rowcount = 0 ;     // number of rows
   int                  i, j, k, tmp;
   BYTE                 *ptr;
   int                  colcount;

   colcount =  FNC_TblOpGetColCount(0, 'W');
   Output_columns = FNC_malloc ( TblOpSIZECOLDEF( colcount ) );
   /* initialize output columns */
   TblOpINITCOLDEF(Output_columns, colcount);
   FNC_TblOpGetColDef(0, 'W', Output_columns);

 
   Input_columns = FNC_malloc ( TblOpSIZECOLDEF( FNC_TblOpGetColCount(0, 'R') ) );

   TblOpINITCOLDEF( Input_columns, FNC_TblOpGetColCount(0, 'R') );
   FNC_TblOpGetColDef(0, 'R', Input_columns);

   aggregates = FNC_malloc( sizeof(float) * Output_columns->num_columns );
   for (i=0; i<Output_columns->num_columns; i++)
   {
      aggregates[i] = 0;
   }
   /* get indices from contract context */
   index = FNC_malloc( FNC_TblOpGetContractLength() );
   FNC_TblOpGetContractDef(index, FNC_TblOpGetContractLength(), &tmp );
   /* The basic row iterator would be structured as follows */

   Input_handle = FNC_TblOpOpen(0, 'R', TBLOP_NOOPTIONS);   // start iterator for input stream
   Output_handle = FNC_TblOpOpen(0, 'W', TBLOP_NOOPTIONS);   // start iterator for output stream

   while ( FNC_TblOpRead(Input_handle) == TBLOP_SUCCESS)
   {
      rowcount++;
      // update aggregate for each column
      for (i=0; i<Output_columns->num_columns; i++)
      {
    
         FNC_TblOpGetAttributeByNdx(Input_handle, index[i], (void **) &ptr, &null_ind, &length);
         switch (Input_columns->column_types[index[i]].datatype)
            {
               case BYTEINT_DT: aggregates[i] += *((signed char *) ptr); break;
               case SMALLINT_DT: aggregates[i] += *((short *) ptr); break;
               case INTEGER_DT: aggregates[i] += *((int *) ptr); break;
               case BIGINT_DT: aggregates[i] += *((long long *) ptr); break;
            }
      }
   }


   for (i=0; i<Output_columns->num_columns; i++)
   {
      if (Output_columns->column_types[i].column[0] == 'A')
      {
         if (rowcount != 0)
         {
            aggregates[i] = aggregates[i] / ((double) rowcount);
         }
      }
      FNC_TblOpBindAttributeByNdx(Output_handle, i, aggregates+i, 0, sizeof(double));
   }


   FNC_TblOpWrite(Output_handle);
   FNC_TblOpClose(Input_handle);
   FNC_TblOpClose(Output_handle);
   FNC_free(Output_columns);
   FNC_free(Input_columns);
   FNC_free(aggregates);
   FNC_free(index);

}
