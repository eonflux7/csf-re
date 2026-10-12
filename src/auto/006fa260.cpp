#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, ScreenEffects_ProcessRange, 0x006FA050,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static uint32_t* __fastcall S_004482c0_ProcessRange(
    S_004482c0* self, void* /*edx*/,
    uint32_t* first, uint32_t* second, uint32_t* third)
{
    return ScreenEffects_ProcessRange(
        first, second, third,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(third)));
}

HOOK(0x006FA260, S_004482c0_ProcessRange, "auto");
