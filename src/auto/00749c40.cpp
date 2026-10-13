#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00749b70_GetPlayerId, 0x00749B70, (void* self, void* edx, int a2));

// Packet-listed reference for the call site at 0x749C56 (the callee's own address).
static const auto* S_00749b70_GetPlayerIdAddr = CSF_VTBL(0x00749B70);

// Scans player slots 0..15 and returns the first slot whose player id matches, or -1.
static int __fastcall S_00749b70_FindPlayerSlot(S_00749b70* self, void* /*edx*/, int playerId)
{
    int foundSlot = -1;
    for (int slot = 0; slot < 16; ++slot)
    {
        if (foundSlot != -1)
            break;
        if (S_00749b70_GetPlayerId(self, nullptr, slot) == playerId)
            foundSlot = slot;
    }
    return foundSlot;
}

HOOK(0x00749C40, S_00749b70_FindPlayerSlot, "auto");
