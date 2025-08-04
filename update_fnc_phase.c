#define SQL_TEXT Latin_Text
#define SQL_STATE_LENGTH 6
#define ERROR_MESSAGE_LENGTH 257

#include <sqltypes_td.h>
#include <string.h>

char tbuf[256];

typedef struct {
    int cnt;
} local_ctx;

void fnc_phase(
    INTEGER *in1,
    VARCHAR_LATIN *in2,
    INTEGER *out1,
    VARCHAR_LATIN *out2,
    int* i_i1,
    int* i_i2,
    int* i_o1,
    int* i_o2,
    char sqlstate[SQL_STATE_LENGTH],
    SQL_TEXT fncname[FNC_MAXNAMELEN],
    SQL_TEXT sfncname[FNC_MAXNAMELEN],
    SQL_TEXT errormsg[ERROR_MESSAGE_LENGTH])
{
    FNC_Phase Phase;
    local_ctx *state_info;

    // Ensure the function is called in the supported context
    if (FNC_GetPhase(&Phase) != TBL_MODE_VARY) {
        strcpy(sqlstate, "U0005");
        strcpy((char *)errormsg, "Table function being called in unsupported mode.");
        return;
    }

    switch (Phase) {
        case TBL_PRE_INIT:
            state_info = FNC_TblAllocCtx(sizeof(local_ctx));
            break;

        case TBL_INIT:
            state_info = FNC_TblGetCtx();
            state_info->cnt = 1;
            break;

        case TBL_BUILD:
            state_info = FNC_TblGetCtx();
            if (state_info->cnt == 0) {
                strcpy(sqlstate, "02000");  // End of data
                return;
            }
            *out1 = *in1;
            strcpy((char*)out2, (char*)in2);
            state_info->cnt = 0;
            break;

        case TBL_FINI:
            break;

        case TBL_END:
            break;
    }
}
