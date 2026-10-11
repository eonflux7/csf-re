#include <cstdint>
#include "hook.h"

static int __cdecl GameConfig_IsStateCodeAllowed(std::uint32_t stateCode)
{
    return stateCode < 4 || stateCode == 18 || stateCode == 31;
}

HOOK(0x00755810, GameConfig_IsStateCodeAllowed, "auto");
