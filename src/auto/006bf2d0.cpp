#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixOptimize, 0x00624560, (int a1, float* a2));

static int __fastcall GfxFrames_OptimizeFrameMatrix(void* self, void* /*edx*/)
{
    uint32_t frameMatrixBase = CSF_AT(uint32_t, self, 0x0C);
    return RwMatrixOptimize(static_cast<int>(frameMatrixBase + 0x10), nullptr);
}

HOOK(0x006BF2D0, GfxFrames_OptimizeFrameMatrix, "auto");
