#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, GfxScreenEffects_ForwardRangeWithContext, 0x006F86F0,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static uint32_t* __fastcall S_0051aa70_ForwardRange(
    S_0051aa70* self, void* /*edx*/,
    uint32_t* a1, uint32_t* a2, uint32_t* a3)
{
    return GfxScreenEffects_ForwardRangeWithContext(
        a1, a2, a3,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(a3));
}

HOOK(0x006F88D0, S_0051aa70_ForwardRange, "auto");
