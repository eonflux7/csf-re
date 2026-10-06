#include "globals.h"
#include "hook.h"

static const char* __cdecl GetProcessTypeName()
{
    return CSF_STR(0x00838114, "TProcesar");
}

HOOK(0x004AA1D0, GetProcessTypeName, "auto");
