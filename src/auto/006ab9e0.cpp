#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(double, __cdecl, sub_411FB0, 0x00411FB0, (float a1));

// Copies the three-component vector stored at this (dword, float, int) into a
// local buffer, normalizes that copy in place, then passes the Y component on
// to sub_411FB0. The result of sub_411FB0 is left in st0 and returned unchanged.
static double __fastcall S_00526d40_NormalizeCopyAndReportY(S_00526d40* self, void* /*edx*/)
{
    struct Vector3Copy {
        uint32_t x;
        float y;
        int z;
    };

    float yValue = CSF_AT(float, self, 0x4);

    Vector3Copy v3;
    v3.x = self->field_0000;
    v3.y = yValue;
    v3.z = CSF_AT(int, self, 0x8);

    NormalizeVector3(&v3, nullptr);
    return sub_411FB0(yValue);
}

HOOK(0x006AB9E0, S_00526d40_NormalizeCopyAndReportY, "auto");
