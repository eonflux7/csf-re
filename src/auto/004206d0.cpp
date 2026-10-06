#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the constant class/type name string "TPreAction".
static const char* __cdecl GetTPreActionName()
{
    return "TPreAction";
}

HOOK(0x4206D0, GetTPreActionName, "auto");
