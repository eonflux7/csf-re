#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_6E8120 (cdecl, 2 args), maps a key to an index.
CSF_FUNC(int, __cdecl, HAnim_LookupBoneIndex, 0x006E8120, (int a1, int a2));

// Finds the 36-byte entry whose dword at +0x18 equals the key returned by
// HAnim_LookupBoneIndex. Key 0 returns the table base stored at +0x04.
static int __fastcall C_00861e98_FindEntryById(C_00861e98* self, void* /*edx*/, int a2)
{
    if (a2 == 0)
        return CSF_AT(int, self, 0x4);

    int key = HAnim_LookupBoneIndex(CSF_AT(int, self, 0x14), a2);
    uint32_t count = CSF_AT(uint32_t, self, 0xC);
    if (count <= 1)
        return 0;

    uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x4)));
    uint32_t index = 1;
    uint8_t* entry = table + 36;
    while (CSF_AT(int, entry, 0x18) != key)
    {
        entry += 36;
        if (++index >= count)
            return 0;
    }
    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x006E9030, C_00861e98_FindEntryById, "auto");
