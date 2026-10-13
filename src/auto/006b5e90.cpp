#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __stdcall, GfxSkin_ReleaseBufferElements, 0x006B4DC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

// Clears the skin-buffer block at +0x34 and the texture-record block at +0x04,
// along with the two side pointers at +0x14, and returns the final texture result.
static int __fastcall ClearTextureAndSkinBuffers(void* self, void* /*edx*/)
{
    int skinBuffer = CSF_AT(int, self, 0x34);
    if (skinBuffer != 0)
    {
        GfxSkin_ReleaseBufferElements(skinBuffer, CSF_AT(int, self, 0x38));
        if (CSF_AT(int, self, 0x34) != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x34)));
    }
    CSF_AT(int, self, 0x34) = 0;
    CSF_AT(int, self, 0x38) = 0;
    CSF_AT(int, self, 0x3C) = 0;

    if (CSF_AT(int, self, 0x14) != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x14)));
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;

    int result = CSF_AT(int, self, 0x04);
    if (result != 0)
    {
        GfxTextureLoader_ResetRecordRange(reinterpret_cast<int*>(result),
                                          reinterpret_cast<int*>(CSF_AT(int, self, 0x08)));
        result = CSF_AT(int, self, 0x04);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x04)));
    }
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    return result;
}

HOOK(0x006B5E90, ClearTextureAndSkinBuffers, "auto");
