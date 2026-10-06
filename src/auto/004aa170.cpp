#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the pointer to the constant string "TEsperar" (0x837B04).
static const char* __cdecl GetEsperarString()
{
    return CSF_STR(0x837B04, "TEsperar");
}

HOOK(0x004AA170, GetEsperarString, "auto");
