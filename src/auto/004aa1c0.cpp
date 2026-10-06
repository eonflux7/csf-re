#include "globals.h"
#include "hook.h"

static const char* __cdecl GetExplosiveTypeName()
{
    return CSF_STR(0x00838084, "TExplosivo");
}

HOOK(0x004AA1C0, GetExplosiveTypeName, "auto");
