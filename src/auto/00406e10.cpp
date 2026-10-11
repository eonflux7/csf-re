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

CSF_FUNC(double, __cdecl, sub_4068E0, 0x004068E0, (float a1, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fDegToRadFactor, 0x0082D754);

// Returns |Angle_ComputeDelta(entry angle, entry value)| for an active entry.
// Entry layout (44 bytes): +0x00 active flag, +0x20 float value, +0x24 float second value.
// When the value is FLT_MAX, the angle is fetched from the object at this+8 through its vtable.
static double __fastcall AngleTable_GetAbsoluteAngleForIndex(void* self, void* /*edx*/, int index)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C)) + 44 * index;

    if (CSF_AT(uint8_t, entry, 0) == 0)
        return g_fZero;

    if (CSF_AT(int32_t, entry, 0x20) == 0x7F7FFFFF)
    {
        void* sourceObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
        void* firstChild = ((void* (*)(void*, void*))CSF_AT(void**, sourceObject, 0)[10])(sourceObject, nullptr);
        void* secondChild = ((void* (*)(void*, void*))CSF_AT(void**, firstChild, 0)[6])(firstChild, nullptr);
        double degrees = ((double (*)(void*, void*))CSF_AT(void**, secondChild, 0)[10])(secondChild, nullptr);
        float radians = (float)degrees * g_fDegToRadFactor;
        return std::fabs(sub_4068E0(radians, CSF_AT(float, entry, 0x24)));
    }

    return std::fabs(sub_4068E0(CSF_AT(float, entry, 0x20), CSF_AT(float, entry, 0x24)));
}

HOOK(0x00406E10, AngleTable_GetAbsoluteAngleForIndex, "auto");
