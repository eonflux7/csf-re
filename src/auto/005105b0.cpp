#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));

// Walks a range of 80-byte entries starting at a1 + 4 and resets the two
// string storages of each entry (at +44 and +0). Stops when cursor - 4 == a2.
static int __stdcall CMenuSaveGame_ResetStringStorageRange(int a1, int a2)
{
    int result = a1;
    if (a1 != a2)
    {
        int cursor = a1 + 4;
        do
        {
            CMenuSaveGame_ResetStringStorage(
                reinterpret_cast<C_0082d410*>(static_cast<uintptr_t>(cursor + 44)), nullptr);
            CMenuSaveGame_ResetStringStorage(
                reinterpret_cast<C_0082d410*>(static_cast<uintptr_t>(cursor)), nullptr);
            cursor += 80;
            result = cursor - 4;
        }
        while (cursor - 4 != a2);
    }
    return result;
}

HOOK(0x005105B0, CMenuSaveGame_ResetStringStorageRange, "auto");
