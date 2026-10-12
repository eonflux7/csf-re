#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, sub_4DDF20, 0x004DDF20, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4DE5B0, 0x004DE5B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Stores two values from a2 into the object, frees every non-null first dword of the
// 16-byte entry array (the array header holds the entry count at -4), frees the header,
// clears the array pointer, then runs the object's two follow-up methods.
static char __fastcall S_004ddf20_SetSourceAndClearEntries(S_004ddf20* self, void* /*edx*/, uint32_t* a2)
{
    self->field_0020 = a2[2];
    uint32_t arrayBase = self->field_001c;
    self->field_0024 = a2[0];

    if (arrayBase != 0)
    {
        int entryCount = *reinterpret_cast<int*>(arrayBase - 4);
        uint32_t headerAddr = arrayBase - 4;

        // Entries are visited from last to first; nothing happens when the count is not positive.
        for (int i = entryCount - 1; i >= 0; --i)
        {
            uint32_t* entry = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(arrayBase) + 16u * static_cast<uint32_t>(i));
            uint32_t ownedPointer = entry[0];
            if (ownedPointer != 0)
            {
                Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(ownedPointer)));
                entry[0] = 0;
            }
        }

        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(headerAddr)));
        self->field_001c = 0;
    }

    // Original code references S_004ddf20's vtable here (via the callee at 0x4DDF20); no store is made.
    (void)CSF_VTBL(0x004DDF20);

    sub_4DDF20(self, nullptr);
    return sub_4DE5B0(self, nullptr);
}

HOOK(0x004DE680, S_004ddf20_SetSourceAndClearEntries, "auto");
