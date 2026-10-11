#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value_8BB4B8, 0x008BB4B8);

CSF_FUNC(unsigned int, __fastcall, GfxSkin_ForwardToGlobalContext, 0x006C5C10, (void* self, void* edx, int a2));

static unsigned int __cdecl GfxSkin_CallWithGlobalContext(int a1)
{
    void* globalContext = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB4B8));
    return GfxSkin_ForwardToGlobalContext(globalContext, nullptr, a1);
}

HOOK(0x006C5C90, GfxSkin_CallWithGlobalContext, "auto");
