#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_6AE040, 0x006AE040, (void* self, void* edx, int a2, int a3, float* a4, float* a5));

static void __fastcall GfxMatrix_ForwardVec3AtOffset30(S_006add20* self, void* /*edx*/, int a2, int a3, float* a4)
{
    // Copy the 3-float vector stored at (self->field_0000 + 0x30) into a local,
    // then forward it together with the original arguments.
    uint32_t vectorAddress = self->field_0000 + 48;
    float vector[3];
    vector[0] = *reinterpret_cast<float*>(vectorAddress);
    vector[1] = *reinterpret_cast<float*>(vectorAddress + 4);
    vector[2] = *reinterpret_cast<float*>(vectorAddress + 8);

    sub_6AE040(self, nullptr, a2, a3, a4, vector);
}

HOOK(0x006AE460, GfxMatrix_ForwardVec3AtOffset30, "auto");
