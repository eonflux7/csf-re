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

// Returns the constant size (0x10 bytes) associated with this object type.
static int __fastcall C_00861a78_GetSize(C_00861a78* /*self*/, void* /*edx*/)
{
    return 16;
}

HOOK(0x479390, C_00861a78_GetSize, "auto");
