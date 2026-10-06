#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0047B0D0 (6 bytes) __cdecl, ret 0
// Returns the constant name of the "Girar" (turn/rotate) task function.
static const char* __cdecl GetTFunctionGirarName()
{
    return CSF_STR(0x00831E68, "TFunctionGirar");
}

HOOK(0x0047B0D0, GetTFunctionGirarName, "auto");
