#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall WeaponFirearms_SetEffectPosition(
    void* self, void* /*edx*/, int effectId,
    const std::uint32_t* position, char enabled)
{
    CSF_AT(int, self, 0x54) = effectId;
    CSF_AT(std::uint32_t, self, 0x58) = position[0];
    CSF_AT(std::uint32_t, self, 0x5C) = position[1];
    const std::uint32_t lastComponent = position[2];
    CSF_AT(std::uint32_t, self, 0x60) = lastComponent;
    CSF_AT(char, self, 0x15) = enabled;
    return static_cast<std::int32_t>(lastComponent);
}

HOOK(0x00476330, WeaponFirearms_SetEffectPosition, "auto");
