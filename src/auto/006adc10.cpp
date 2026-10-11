#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixTranslate, 0x00624B30, (int a1, float* a2, int a3));

// Resets the RW matrix at *self to identity-diagonal form with flags 0x20003,
// then applies a translation by the vector a2 via RwMatrixTranslate.
static int __fastcall GfxMatrix_ResetAndTranslate(void* self, void* /*edx*/, int a2)
{
    uint8_t* matrix = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0));

    CSF_AT(float, matrix, 0x28) = 1.0f;
    CSF_AT(float, matrix, 0x14) = 1.0f;
    CSF_AT(float, matrix, 0x00) = 1.0f;
    CSF_AT(uint32_t, matrix, 0x10) = 0;
    CSF_AT(uint32_t, matrix, 0x08) = 0;
    CSF_AT(uint32_t, matrix, 0x04) = 0;
    CSF_AT(uint32_t, matrix, 0x24) = 0;
    CSF_AT(uint32_t, matrix, 0x20) = 0;
    CSF_AT(uint32_t, matrix, 0x18) = 0;
    CSF_AT(uint32_t, matrix, 0x38) = 0;
    CSF_AT(uint32_t, matrix, 0x34) = 0;
    CSF_AT(uint32_t, matrix, 0x30) = 0;
    CSF_AT(uint32_t, matrix, 0x0C) |= 0x20003u;

    return RwMatrixTranslate(static_cast<int>(reinterpret_cast<uintptr_t>(matrix)),
                             reinterpret_cast<float*>(a2),
                             2);
}

HOOK(0x006ADC10, GfxMatrix_ResetAndTranslate, "auto");
