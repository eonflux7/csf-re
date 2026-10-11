#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));

CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_DegreesPerRadian, 0x0082D7C4);

// Rotates the matrix of sub-entry a2 (stride 28 bytes) around the Z axis by a3 radians.
// Then, if the entry's object is non-null, calls its vtable slot 18 with the Z axis and the angle in degrees.
static int __fastcall RotateSubEntryAroundAxisZ(uint8_t* self, void* /*edx*/, int a2, float a3)
{
    int result = GfxMatrix_RotateAroundAxisZ(
        reinterpret_cast<int*>(CSF_AT(uint8_t*, self, 0x0C) + a2 * 28 + 8), nullptr, a3);

    // The table pointer is reloaded after the matrix call, as the original does.
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    void* entryObject = reinterpret_cast<void*>(CSF_AT(int, table, a2 * 28));
    if (entryObject != nullptr)
    {
        float angleDegrees = a3 * g_DegreesPerRadian;
        // Vtable slot 0x48 / 4 = 18. The object is still in ecx at the call, and the
        // callee pops the two stack arguments (&g_AxisZ, angle).
        return ((int(__fastcall*)(void*, void*, float*, float))CSF_AT(void**, entryObject, 0)[18])(
            entryObject, nullptr, &g_AxisZ, angleDegrees);
    }
    return result;
}

HOOK(0x408000, RotateSubEntryAroundAxisZ, "auto");
