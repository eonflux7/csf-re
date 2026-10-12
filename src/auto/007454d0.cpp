#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0,
         (void* self, void* edx, bool* a2));

static void* __stdcall NetPlayerDDL_ResolvePlayer(C_008622e8* player)
{
    return NetPlayerDDL_ResolveObject(
        player, nullptr,
        reinterpret_cast<bool*>(reinterpret_cast<uint8_t*>(&player) + 3));
}

HOOK(0x007454D0, NetPlayerDDL_ResolvePlayer, "auto");
