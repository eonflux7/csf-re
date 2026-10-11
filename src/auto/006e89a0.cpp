#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the HAnim keyframe entry (16-byte records) at frameIndex of this->table->entries.
// outFirst receives entry[0]; outSecond receives entry[3] and the return value becomes that value.
// Returns 0 when this->+0x1C is zero.
static uint32_t* __fastcall GetHanimKeyframeEntry(void* self, void* /*edx*/,
                                                  int frameIndex,
                                                  uint32_t* outFirst,
                                                  uint32_t* outSecond)
{
    uint32_t* result = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C));
    if (result) {
        void* tableOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
        uint32_t entriesBase = CSF_AT(uint32_t, tableOwner, 0x10);
        uint32_t entryAddress = entriesBase + (static_cast<uint32_t>(frameIndex) << 4);
        uint32_t* entry = reinterpret_cast<uint32_t*>(entryAddress);

        result = entry;
        if (outFirst) {
            *outFirst = entry[0];
        }
        if (outSecond) {
            uint32_t secondValue = entry[3];
            result = reinterpret_cast<uint32_t*>(secondValue);
            *outSecond = secondValue;
        }
    }
    return result;
}

HOOK(0x6E89A0, GetHanimKeyframeEntry, "auto");
