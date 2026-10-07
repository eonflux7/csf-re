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

// Returns the 32-bit value stored at offset 0x150.
static int __fastcall C_00865e38_GetField0150(C_00865e38* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0150);
}

HOOK(0x0072D110, C_00865e38_GetField0150, "auto");
