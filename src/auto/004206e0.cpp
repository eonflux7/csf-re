#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the class name literal of the TAction class.
static const char* __cdecl TAction_GetName()
{
    return CSF_STR(0x82EC58, "TAction");
}

HOOK(0x4206E0, TAction_GetName, "auto");
