#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Approximate float comparison used by the game (cdecl, three float args).
CSF_FUNC(int, __cdecl, FloatsNearlyEqual, 0x0042D660, (float a, float b, float epsilon));

// Compares the five dword fields of two entries exactly, then compares the
// trailing float (+0x14) with a tolerance of 1e-6 (0x358637BD).
static int __fastcall CMenuMpScoreBar_IsSameEntry(void* self, void* /*edx*/, void* other)
{
    if (CSF_AT(int32_t, self, 0x00) != CSF_AT(int32_t, other, 0x00))
        return 0;
    if (CSF_AT(int32_t, self, 0x04) != CSF_AT(int32_t, other, 0x04))
        return 0;
    if (CSF_AT(int32_t, self, 0x08) != CSF_AT(int32_t, other, 0x08))
        return 0;
    if (CSF_AT(int32_t, self, 0x0C) != CSF_AT(int32_t, other, 0x0C))
        return 0;
    if (CSF_AT(int32_t, self, 0x10) != CSF_AT(int32_t, other, 0x10))
        return 0;

    const float selfValue = CSF_AT(float, self, 0x14);
    const float otherValue = CSF_AT(float, other, 0x14);
    if (!FloatsNearlyEqual(selfValue, otherValue, 0.000001f))
        return 0;

    return 1;
}

HOOK(0x00541030, CMenuMpScoreBar_IsSameEntry, "auto");
