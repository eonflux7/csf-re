#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the pointer to the string constant "TAcudiendo" (0x0083BE70).
static const char* __cdecl GetTAcudiendo()
{
    return CSF_STR(0x83BE70, "TAcudiendo");
}

HOOK(0x4D48F0, GetTAcudiendo, "auto");
