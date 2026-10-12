#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry reset routine (__fastcall, per packet bind line).
CSF_FUNC(unsigned int, __fastcall, CampaignEntry_Reset, 0x0043E360, (void* self, void* edx));

// Game heap free with debug info (__cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static constexpr std::size_t kCampaignEntryStride = 0xC4;

// Resets every campaign entry in the [begin, end) range (stride 0xC4), frees the
// backing block if one exists, and clears the three list pointers.
static int __fastcall ClearCampaignEntryList(void* self, void* /*edx*/)
{
    uint8_t* cursor = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    if (cursor)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        for (; cursor != end; cursor += kCampaignEntryStride)
        {
            CampaignEntry_Reset(cursor, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04))));
        }
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return 0;
}

HOOK(0x0043F890, ClearCampaignEntryList, "auto");
