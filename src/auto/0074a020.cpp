#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00749b70_FindPlayerEntry, 0x00749C40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00749b70_GetPlayerId, 0x00749B70, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00749b70_SetEntryValue, 0x00749C70, (S_00749b70* self, void* , int index, int value));

// Registers player a2 in this spawn table: returns the existing entry if the
// player is already present, otherwise stores it in the first free slot (0..15).
static int __fastcall S_00749b70_RegisterPlayer(S_00749b70* self, void* /*edx*/, int a2)
{
    int result = S_00749b70_FindPlayerEntry(self, nullptr, a2);
    if (result != -1)
        return result;

    int freeSlot = -1;
    int slot = 0;
    while (freeSlot == -1)
    {
        result = S_00749b70_GetPlayerId(self, nullptr, slot);
        if (result == -1)
            freeSlot = slot;
        if (++slot >= 16)
            break;
    }

    if (freeSlot == -1)
        return result;

    return S_00749b70_SetEntryValue(self, nullptr, freeSlot, a2);
}

HOOK(0x0074A020, S_00749b70_RegisterPlayer, "auto");
