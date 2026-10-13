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

CSF_FUNC(char, __fastcall, TextDatabase_TestHorizontalPosition, 0x00770760, (void* self, void* edx, float a2, float a3));

// Returns nonzero when the point (a2[0], a2[1], a2[2]) lies inside this entry's
// vertical band [field_0050, field_0050 + field_0054) and passes the horizontal test.
// NaN fails the comparisons, matching the x87 fcom/fcompp checks in the original.
static int __fastcall TextDatabase_SceneEntryContainsPosition(S_00770760* self, void* /*edx*/, float* a2)
{
    if (!(a2[1] >= self->field_0050))
        return 0;

    if (!(a2[1] < self->field_0054 + self->field_0050))
        return 0;

    if (!TextDatabase_TestHorizontalPosition(self, nullptr, a2[0], a2[2]))
        return 0;

    return 1;
}

HOOK(0x007707E0, TextDatabase_SceneEntryContainsPosition, "auto");
