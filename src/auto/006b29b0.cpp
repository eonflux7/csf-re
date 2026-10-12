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

// Returns 1 when bit 0x100 of the object's flags field (+0x064) is set, otherwise 0.
static int __fastcall C_00860288_IsFlag0x100Set(C_00860288* self, void* /*edx*/)
{
    return (self->field_0064 & 0x100) == 0x100 ? 1 : 0;
}

HOOK(0x006B29B0, C_00860288_IsFlag0x100Set, "auto");
