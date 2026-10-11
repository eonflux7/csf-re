#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00755810: __cdecl check (ret 0) that tests whether a game state code is allowed.
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

// 0x0047B260: __stdcall wrapper (ret 4). Forwards its single argument to
// GameConfig_IsStateCodeAllowed and returns the result unchanged.
static int __stdcall AiAttackTasks_CheckStateCodeAllowed(int stateCode)
{
    return GameConfig_IsStateCodeAllowed(static_cast<std::uint32_t>(stateCode));
}

HOOK(0x47B260, AiAttackTasks_CheckStateCodeAllowed, "auto");
