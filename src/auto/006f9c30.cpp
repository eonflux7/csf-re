#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

static C_00860178* __cdecl GfxScreenEffects_CreateEffect()
{
    auto* effect = static_cast<C_00860178*>(Mem_Alloc(200));
    if (!effect)
        return nullptr;

    C_00860178_Construct(effect, nullptr);
    CSF_AT(void**, effect, 0x04) = CSF_VTBL(0x0082D428);
    CSF_AT(std::uint8_t, effect, 0x08) = 0;
    CSF_AT(std::uint32_t, effect, 0x54) = 0;
    CSF_AT(std::uint32_t, effect, 0x58) = 0;
    CSF_AT(std::uint32_t, effect, 0x5C) = 0;
    CSF_AT(void**, effect, 0x00) = CSF_VTBL(0x00862B04);
    CSF_AT(void**, effect, 0x04) = CSF_VTBL(0x00862AE8);
    return effect;
}

HOOK(0x006F9C30, GfxScreenEffects_CreateEffect, "auto");
