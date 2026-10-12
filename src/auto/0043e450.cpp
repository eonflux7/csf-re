#include <cstdint>
#include "globals.h"
#include "hook.h"

static constexpr uint32_t kCampaignEntrySize = 196;  // 0xC4 bytes per entry

static void* __fastcall CampaignTable_FindEntryById(void* self, void* /*edx*/, int wantedId)
{
    uint32_t entryCount = 0;
    uint32_t entriesBegin = CSF_AT(uint32_t, self, 0x34);
    if (entriesBegin != 0) {
        entryCount = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x38) - entriesBegin) / 196);
    }
    if (entryCount == 0) {
        return nullptr;
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(entriesBegin);
    for (uint32_t index = 0; index < entryCount; ++index) {
        if (wantedId == static_cast<int>(*reinterpret_cast<uint32_t*>(entry))) {
            return entry;
        }
        entry += kCampaignEntrySize;
    }
    return nullptr;
}

HOOK(0x0043E450, CampaignTable_FindEntryById, "auto");
