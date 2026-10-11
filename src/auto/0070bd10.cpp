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

CSF_GLOBAL(float, g_HalfDurationScale, 0x82D444);

static int __fastcall C_00863760_SetHalfDurationFromArg(C_00863760* self, void* /*edx*/, int a2)
{
    // a2 points at a record whose float at +4 is scaled by g_HalfDurationScale
    // and stored into this->field_0014. The original returns a2 unchanged.
    float sourceValue = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 4);
    self->field_0014 = sourceValue * g_HalfDurationScale;
    return a2;
}

HOOK(0x70BD10, C_00863760_SetHalfDurationFromArg, "auto");
