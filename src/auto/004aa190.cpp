#include "globals.h"
#include "hook.h"

static const char* __cdecl GetProtectionSearchTypeName()
{
    return CSF_STR(0x00837C24, "TBuscarProteccion");
}

HOOK(0x004AA190, GetProtectionSearchTypeName, "auto");
