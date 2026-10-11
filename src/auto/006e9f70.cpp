#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, ReleaseTrackPayload, 0x00707550, (void* self, void* edx));

// Returns the track-list entry at index i (re-read from the list base each time).
static uintptr_t GetTrackListEntry(C_00861e98* self, int i)
{
    void* listBase = reinterpret_cast<void*>(CSF_AT(uintptr_t, self, 0x4c));
    return CSF_AT(uintptr_t, listBase, 4 * i);
}

// Frees every track entry's secondary buffer and its payload, clearing the slots,
// then clears bit 2 of the flags word and resets the scale to 1.0f.
static unsigned int __fastcall C_00861e98_ReleaseKeyframeTracks(C_00861e98* self, void* /*edx*/)
{
    int i = 0;
    while (i < CSF_AT(int, reinterpret_cast<void*>(CSF_AT(uintptr_t, self, 0x14)), 4))
    {
        uintptr_t entry = GetTrackListEntry(self, i);
        if (entry)
        {
            uintptr_t extraBuffer = CSF_AT(uintptr_t, reinterpret_cast<void*>(entry), 0x24);
            if (extraBuffer)
            {
                Mem_Free(reinterpret_cast<void*>(extraBuffer));
                uintptr_t entryAgain = GetTrackListEntry(self, i);
                CSF_AT(uintptr_t, reinterpret_cast<void*>(entryAgain), 0x24) = 0;
            }

            uintptr_t payload = GetTrackListEntry(self, i);
            if (payload)
            {
                ReleaseTrackPayload(reinterpret_cast<void*>(payload), nullptr);
                Mem_Free(reinterpret_cast<void*>(payload));
                void* listBase = reinterpret_cast<void*>(CSF_AT(uintptr_t, self, 0x4c));
                CSF_AT(uintptr_t, listBase, 4 * i) = 0;
            }
        }
        ++i;
    }

    unsigned int result = CSF_AT(uint32_t, self, 0x58) & 0xFFFFFFFBu;
    CSF_AT(uint32_t, self, 0x58) = result;
    CSF_AT(float, self, 0x54) = 1.0f;
    return result;
}

HOOK(0x6E9F70, C_00861e98_ReleaseKeyframeTracks, "auto");
