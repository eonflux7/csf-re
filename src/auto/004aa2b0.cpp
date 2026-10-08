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

// 0x004AA2B0 (6 bytes)
// Returns the pointer to the constant string "TEntrar" (0x00838DBC).
static const char* __cdecl GetTEntrarString()
{
    CSF_STR(0x838DBC, "TEntrar");
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x838DBC));
}

HOOK(0x4AA2B0, GetTEntrarString, "auto");
