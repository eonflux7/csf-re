#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Stores a three-word keyframe sample into the compressed-track entry selected by
// the sample's slot index. Returns the sample's second word (or the looked-up slot
// pointer when nothing is stored).
static uint32_t* __fastcall AnimCompress_StoreKeyframe(uint32_t* self, void* /*edx*/, uint32_t* a2, uint8_t* a3) {
    int32_t slotIndex = CSF_AT(int32_t, a2, 0x18);
    uint32_t* result = reinterpret_cast<uint32_t*>(self[slotIndex + 1]);

    if (*result != 0) {
        int32_t entryIndex = CSF_AT(int32_t, result, 0x08);
        if (entryIndex != -1) {
            uint8_t* track = CSF_AT(uint8_t*, a3, 0x20);
            int32_t entryStride = CSF_AT(int32_t, track, 0x24);
            uint32_t* entry = reinterpret_cast<uint32_t*>(track + 0x64 + entryIndex * entryStride);

            entry[0] = CSF_AT(uint32_t, a2, 0x0C);
            uint32_t secondWord = CSF_AT(uint32_t, a2, 0x10);
            entry[1] = secondWord;
            result = reinterpret_cast<uint32_t*>(secondWord);
            entry[2] = CSF_AT(uint32_t, a2, 0x14);
        }
    }
    return result;
}

HOOK(0x6C2460, AnimCompress_StoreKeyframe, "auto");
