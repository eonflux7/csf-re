#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixScale, 0x006249A0, (int a1, float* a2, int a3));

static int __fastcall GfxMatrix_Scale(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    uint32_t scaleVector[3];
    scaleVector[0] = static_cast<uint32_t>(a2);
    scaleVector[1] = static_cast<uint32_t>(a3);
    scaleVector[2] = static_cast<uint32_t>(a4);

    int matrixHandle = CSF_AT(int, self, 0);
    return RwMatrixScale(matrixHandle, reinterpret_cast<float*>(scaleVector), 2);
}

HOOK(0x006ADB60, GfxMatrix_Scale, "auto");
