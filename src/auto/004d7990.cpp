#include "globals.h"
#include "hook.h"

static const char* __cdecl GetTLanzarObjetoAPosName()
{
    return CSF_STR(0x0083DB80, "TLanzarObjetoAPos");
}

HOOK(0x004D7990, GetTLanzarObjetoAPosName, "auto");
