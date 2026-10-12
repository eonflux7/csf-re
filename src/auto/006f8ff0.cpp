#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, GfxScreenEffects_DestroyElements, 0x006F8810, (int a1, int a2));
CSF_FUNC(char, __fastcall, GfxScreenEffects_ReleaseEffect, 0x007090F0, (void* self, void* edx));

static void __fastcall C_008629a4_Destroy(C_008629a4* self, void* /*edx*/)
{
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x008629A4);
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x00862988);

    GfxScreenEffects_ReleaseEffect(self, nullptr);

    const auto elements = CSF_AT(std::uint32_t, self, 0x54);
    if (elements)
    {
        GfxScreenEffects_DestroyElements(
            static_cast<int>(elements),
            CSF_AT(int, self, 0x58));

        if (CSF_AT(std::uint32_t, self, 0x54))
            Mem_FreeDebug(reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, self, 0x54)));
    }

    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006F8FF0, C_008629a4_Destroy, "auto");
