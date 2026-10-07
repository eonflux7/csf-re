#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the constant string "TMoverse" (address 0x838234).
static const char* __cdecl GetTMoverseString()
{
    return CSF_STR(0x838234, "TMoverse");
}

HOOK(0x4AA1F0, GetTMoverseString, "auto");
