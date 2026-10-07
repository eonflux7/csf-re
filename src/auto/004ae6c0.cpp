#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the identifier of the parapet-attack function ("TFncAtacarDesdeParapeto").
static const char* __fastcall TFncAtacarDesdeParapeto_GetParapetAttackFnName(TFncAtacarDesdeParapeto* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x00839F2C, "TFncAtacarDesdeParapeto");
}

HOOK(0x004AE6C0, TFncAtacarDesdeParapeto_GetParapetAttackFnName, "auto");
