#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisX, 0x006AE190, (int* self, void* , float angle));

CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_DegreesPerRadian, 0x0082D7C4);

// Rotates the matrix of sub-entry a2 (stride 28 bytes) around the X axis by a3 radians.
// Then, if the entry's object is non-null, calls its vtable slot 18 with the X axis and the angle in degrees.
static int __fastcall RotateSubEntryAroundAxisX(uint8_t* self, void* /*edx*/, int a2, float a3)
{
    int result = GfxMatrix_RotateAroundAxisX(
        reinterpret_cast<int*>(CSF_AT(uint8_t*, self, 0x0C) + a2 * 28 + 8), nullptr, a3);

    // The table pointer is reloaded after the matrix call, as the original does.
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    int entryObject = CSF_AT(int, table, a2 * 28);
    if (entryObject != 0)
    {
        float angleDegrees = a3 * g_DegreesPerRadian;
        // Vtable slot 0x48 / 4 = 18. The original loads the object into ecx (push ecx; fstp [esp]
        // overwrites that pushed slot with the degrees), then pushes &g_AxisX and calls [eax+0x48]:
        // ecx = object, stack = (&g_AxisX, degrees), callee pops 8.
        return ((int(__fastcall*)(void*, void*, float*, float))CSF_AT(void**, reinterpret_cast<void*>(entryObject), 0)[18])(
            reinterpret_cast<void*>(entryObject), nullptr, &g_AxisX, angleDegrees);
    }
    return result;
}

HOOK(0x407F60, RotateSubEntryAroundAxisX, "auto");
