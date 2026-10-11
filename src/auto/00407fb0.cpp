#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_DegreesPerRadian, 0x0082D7C4);

// Rotates the matrix of sub-entry a2 (stride 28 bytes) around the Y axis by a3 radians.
// Then, if the entry's object is non-null, calls its vtable slot 18 with the Y axis and the angle in degrees.
static int __fastcall RotateSubEntryAroundAxisY(uint8_t* self, void* /*edx*/, int a2, float a3)
{
    int result = GfxMatrix_RotateAroundAxisY(
        reinterpret_cast<int*>(CSF_AT(uint8_t*, self, 0x0C) + a2 * 28 + 8), nullptr, a3);

    // The table pointer is reloaded after the matrix call, as the original does.
    uint8_t* table = CSF_AT(uint8_t*, self, 0x0C);
    int entryObject = CSF_AT(int, table, a2 * 28);
    if (entryObject != 0)
    {
        float angleDegrees = a3 * g_DegreesPerRadian;
        // Vtable slot 0x48 / 4 = 18. The object is passed in ecx (thiscall), so the
        // call is made through a __fastcall-typed pointer with the object as the first arg.
        void* object = reinterpret_cast<void*>(entryObject);
        return ((int(__fastcall*)(void*, void*, float*, float))CSF_AT(void**, object, 0)[18])(
            object, nullptr, &g_AxisY, angleDegrees);
    }
    return result;
}

HOOK(0x407FB0, RotateSubEntryAroundAxisY, "auto");
