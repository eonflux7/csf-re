#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwMatrixTranslate, 0x00624B30, (int a1, float* a2, int a3));

// __thiscall: this = ecx (pointer to a object whose first dword is passed as the matrix handle), one stack arg (ret 4).
static int __fastcall GfxMatrix_TranslateFrameByOffset(uint32_t* self, void* /*edx*/, int offset) {
    return RwMatrixTranslate(static_cast<int>(*self), reinterpret_cast<float*>(offset), 2);
}

HOOK(0x006ADB90, GfxMatrix_TranslateFrameByOffset, "auto");
