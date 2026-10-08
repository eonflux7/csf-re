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

// Returns the address of the constant "TDefault" string stored in the
// read-only data section (0x83566C).
static const char* __cdecl GetTDefaultString()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x83566C));
}

HOOK(0x4A9EC0, GetTDefaultString, "auto");
