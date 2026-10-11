#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixTranslate, 0x00624B30, (int a1, float* a2, int a3));

static int __fastcall GfxMatrix_Translate(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    uint32_t translateVector[3];
    translateVector[0] = static_cast<uint32_t>(a2);
    translateVector[1] = static_cast<uint32_t>(a3);
    translateVector[2] = static_cast<uint32_t>(a4);

    int matrixHandle = CSF_AT(int, self, 0);
    return RwMatrixTranslate(matrixHandle, reinterpret_cast<float*>(translateVector), 2);
}

HOOK(0x006ADBB0, GfxMatrix_Translate, "auto");
