#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the identifier of the parapet-attack function ("TFncAtacarDesdeParapeto").
static const char* __fastcall C_00839ea8_GetParapetAttackFnName(C_00839ea8* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x00839F2C, "TFncAtacarDesdeParapeto");
}

HOOK(0x004AE6C0, C_00839ea8_GetParapetAttackFnName, "auto");
