#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __cdecl, GfxTextureLoader_ProcessRange, 0x006DE1A0,
         (char* a1, char* a2, char* a3, int a4, int a5));

static char* __fastcall GfxTextureLoader_ForwardRangeWithContext(
    S_0046bea0* self, void* /*edx*/, char* first, char* second, char* third)
{
    return GfxTextureLoader_ProcessRange(
        first, second, third,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(third)));
}

HOOK(0x006DE370, GfxTextureLoader_ForwardRangeWithContext, "auto");
