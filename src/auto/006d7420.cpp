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

// Returns the address of the sub-object / member that lives at offset 0x108
// inside the object (this + 264). No stack arguments, plain getter.
static void* __fastcall C_006D7420_GetSubObjectAt0x108(void* self, void* /*edx*/)
{
    return static_cast<char*>(self) + 0x108;
}

HOOK(0x6D7420, C_006D7420_GetSubObjectAt0x108, "auto");
