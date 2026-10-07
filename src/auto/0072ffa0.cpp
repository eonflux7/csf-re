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

// Returns field_0140 of C_00865e38 (offset 0x140).
static int __fastcall C_00865e38_GetField0140(C_00865e38* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0140);
}

HOOK(0x0072FFA0, C_00865e38_GetField0140, "auto");
