#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GfxTextureLoader_ProcessWithContext, 0x006DECE0,
         (int a1, int a2, int a3, int a4, int a5));

static int __fastcall GfxTextureLoader_ForwardWithContext(
    void* self, void* /*edx*/, int a1, int a2, int a3)
{
    return GfxTextureLoader_ProcessWithContext(
        a1, a2, a3, static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), a3);
}

HOOK(0x006DEEB0, GfxTextureLoader_ForwardWithContext, "auto");
