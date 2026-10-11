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

CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void* edx));
CSF_FUNC(void*, __fastcall, SubObject_GetAtOffset, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SetFacingAngleRadians, 0x00421B30, (void* self, void* edx, float a2, char a3));

// Computes the heading from the object's position delta and applies it to the sub-object at +0x1A8.
static int __fastcall S_004223f0_ApplyHeadingFromPositionDelta(S_004223f0* self, void* /*edx*/)
{
    auto* obj = reinterpret_cast<uint8_t*>(self->field_0008);

    S_00526d40 heading_vector;
    CSF_AT(float, &heading_vector, 0x0) = CSF_AT(float, obj, 0x224) - CSF_AT(float, obj, 0x20c);
    CSF_AT(float, &heading_vector, 0x4) = CSF_AT(float, obj, 0x228) - CSF_AT(float, obj, 0x210);
    CSF_AT(float, &heading_vector, 0x8) = CSF_AT(float, obj, 0x22c) - CSF_AT(float, obj, 0x214);

    float heading_radians = static_cast<float>(S_00526d40_GetVectorAngleRadians(&heading_vector, nullptr));

    uint32_t object_address = self->field_0008;
    void* sub_object = SubObject_GetAtOffset(self, nullptr, static_cast<int>(object_address + 0x1A8));
    return SetFacingAngleRadians(sub_object, nullptr, heading_radians, 1);
}

HOOK(0x004223F0, S_004223f0_ApplyHeadingFromPositionDelta, "auto");
