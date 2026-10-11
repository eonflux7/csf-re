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

CSF_FUNC(bool, __fastcall, ComputeCachedRangeResult, 0x004E55C0, (void* self, void* edx, int a2, int a3));

// Returns the cached verdict stored in a2[0] when it has been resolved (1 = true, 0 = false).
// Otherwise, if the object's flag at +0x4D is set, computes the verdict from the range
// a2[1..] and caches it in a2[0]; if the flag is clear, the verdict is true (not cached).
static char __fastcall C_0083e9d0_ResolveCachedResult(C_0083e9d0* self, void* /*edx*/, int32_t* a2)
{
    if (*a2 != -1)
        return *a2 == 1;
    if (!self->field_004d)
        return 1;
    char result = ComputeCachedRangeResult(self, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(a2 + 2)), a2[1]);
    a2[0] = (result != 0) ? 1 : 0;
    return result;
}

HOOK(0x4E5890, C_0083e9d0_ResolveCachedResult, "auto");
