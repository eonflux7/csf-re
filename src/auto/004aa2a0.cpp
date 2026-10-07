#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the script-visible name of the "pause before action" function.
static const char* __cdecl FncPausaAntesAccion_GetName()
{
    return CSF_STR(0x838C0C, "TFncPausaAntesAccion");
}

HOOK(0x004AA2A0, FncPausaAntesAccion_GetName, "auto");
