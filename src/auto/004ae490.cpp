#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the fixed name string for the "TFncAtacarDesdePosicion" action function.
static const char* __fastcall TFncAtacarDesdePosicion_GetName(TFncAtacarDesdePosicion* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x00839994, "TFncAtacarDesdePosicion");
}

HOOK(0x004AE490, TFncAtacarDesdePosicion_GetName, "auto");
