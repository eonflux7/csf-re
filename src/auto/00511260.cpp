#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, StringAllocatorReset, 0x005112A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));

// Walks a contiguous array of 552-byte records starting 4 bytes past `first`,
// until the record's trailing 4-byte marker (v3 - 4) equals `last`. For each
// record it clears the save-game string storage embedded at +0x1F0 and resets
// the string allocator at the record base.
static int __cdecl ResetStringStorageRange(int first, int last)
{
    int result = first;
    if (first != last)
    {
        int recordBase = first + 4;
        do
        {
            void* storage = reinterpret_cast<void*>(static_cast<uintptr_t>(recordBase + 0x1F0));
            CMenuSaveGame_ResetStringStorage(storage, nullptr);

            StringAllocatorReset(reinterpret_cast<void*>(static_cast<uintptr_t>(recordBase)), nullptr);

            recordBase += 0x228;
            result = recordBase - 4;
        } while (recordBase - 4 != last);
    }
    return result;
}

HOOK(0x511260, ResetStringStorageRange, "auto");
