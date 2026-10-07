#include "hook.h"
#include "globals.h"

// Returns the constant model/name string "TGiraAngOriginal".
static const char* __cdecl GetGiraAngOriginalString()
{
    return CSF_STR(0x0083C0A8, "TGiraAngOriginal");
}

HOOK(0x4D4910, GetGiraAngOriginalString, "auto");
