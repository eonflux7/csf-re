#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the fixed name string for the "TFncAtacarDesdePosicion" action function.
static const char* __fastcall C_00839910_GetName(C_00839910* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x00839994, "TFncAtacarDesdePosicion");
}

HOOK(0x004AE490, C_00839910_GetName, "auto");
