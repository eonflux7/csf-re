#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_00749b70_FindObjectSlot(
    S_00749b70* self, void* /*edx*/, int objectId)
{
    int firstEmptySlot = -1;

    for (int slot = 0; slot < 16; ++slot)
    {
        const int storedId = CSF_AT(int, self, 0x8C + slot * 8);
        if (storedId == objectId)
            return slot;

        if (firstEmptySlot == -1 && storedId == -1)
            firstEmptySlot = slot;
    }

    return firstEmptySlot;
}

HOOK(0x00749F30, S_00749b70_FindObjectSlot, "auto");
