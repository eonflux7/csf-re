#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004AA0A0 (6 bytes, __cdecl, ret 0)
// Returns the static string "TFncGirar".
static const char* __cdecl TFncGirar_GetName()
{
    return "TFncGirar";
}

HOOK(0x4AA0A0, TFncGirar_GetName, "auto");
