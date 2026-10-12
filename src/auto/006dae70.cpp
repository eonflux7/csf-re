#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, ProfilesSavegames_AssignRecordRange, 0x006D9E00, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
CSF_FUNC(char, __fastcall, ProfilesSavegames_GrowAndFill, 0x006DAAD0, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Resizes a 16-byte-element record array (begin at +4, end at +8) so it holds `count` elements.
// Stack args: count (ebp+8), fillValue (ebp+0xC, passed by address to the grow helper), three unused (ret 0x14).
static uint32_t* __fastcall ProfilesSavegames_ResizeRecordsTo(void* self, void* /*edx*/,
                                                              uint32_t count, uint32_t fillValue,
                                                              uint32_t /*stackArg3*/, uint32_t /*stackArg4*/, uint32_t /*stackArg5*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x4);
    uint32_t size = 0;
    if (begin != 0) {
        size = static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, self, 0x8) - begin) >> 4);
    }

    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(count));

    if (size >= count) {
        // Shrinking (or no change) path.
        if (CSF_AT(uint32_t, self, 0x4) != 0) {
            const uint32_t beginNow = CSF_AT(uint32_t, self, 0x4);
            const uint32_t end = CSF_AT(uint32_t, self, 0x8);
            const uint32_t endSize = static_cast<uint32_t>(static_cast<int32_t>(end - beginNow) >> 4);
            if (count < endSize) {
                const uint32_t newEnd = beginNow + 16 * count;
                result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(newEnd));
                if (newEnd != end) {
                    result = ProfilesSavegames_AssignRecordRange(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(end)),
                                                                 reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(end)),
                                                                 result,
                                                                 static_cast<int>(count));
                    CSF_AT(uint32_t, self, 0x8) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(result));
                }
            }
        }
        return result;
    }

    // Growing path: current size is recomputed from the live fields.
    uint32_t currentSize = 0;
    if (CSF_AT(uint32_t, self, 0x4) != 0) {
        currentSize = static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, self, 0x8) - CSF_AT(uint32_t, self, 0x4)) >> 4);
    }
    const char grown = ProfilesSavegames_GrowAndFill(self, nullptr,
                                                     reinterpret_cast<char*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8))),
                                                     count - currentSize,
                                                     &fillValue);
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(grown));
}

HOOK(0x006DAE70, ProfilesSavegames_ResizeRecordsTo, "auto");
