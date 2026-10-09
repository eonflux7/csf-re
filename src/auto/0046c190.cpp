#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionObjectFreeBlock, 0x004714E0, (void* Block));

// Walks (a1..a2) in 8-byte entries; for each non-null object at entry+4, runs its
// virtual slot 0 with flags 0 (no delete), frees it via MissionObjectFreeBlock, then nulls the slot.
static int __cdecl MissionScript_DestroyAndClearPairs(int a1, int a2, int a3, int a4)
{
    (void)a3;
    (void)a4;

    int result = 0;
    uint8_t* cursor = reinterpret_cast<uint8_t*>(a1);
    uint8_t* end = reinterpret_cast<uint8_t*>(a2);

    while (cursor != end)
    {
        S_0069b1c0* object = CSF_AT(S_0069b1c0*, cursor, 4);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])(object, nullptr, 0);
            result = MissionObjectFreeBlock(object);
        }
        CSF_AT(void*, cursor, 4) = nullptr;
        cursor += 8;
    }
    return result;
}

HOOK(0x0046C190, MissionScript_DestroyAndClearPairs, "auto");
