#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value_8BB4B8, 0x008BB4B8);

CSF_FUNC(int, __fastcall, GfxSkin_ProcessSkinWithGlobalContext, 0x006C6D40, (void* self, void* edx, int a1));

static int __cdecl GfxSkin_CallProcessWithGlobalContext(int a1)
{
    void* globalContext = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB4B8));
    return GfxSkin_ProcessSkinWithGlobalContext(globalContext, nullptr, a1);
}

HOOK(0x006C6E20, GfxSkin_CallProcessWithGlobalContext, "auto");
