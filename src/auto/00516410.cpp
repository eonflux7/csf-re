#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, StlRange_TruncateTo, 0x006D9E00, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
CSF_FUNC(unsigned int, __fastcall, S_00514aa0_GrowAndFill, 0x005160B0, (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// Resizes a 16-byte-element array owned by S_00514aa0 so it holds `count` elements.
// Stack args: count (ebp+8), fillValue (ebp+0xC, passed by address to the grow helper), three unused (ret 0x14).
static uint32_t* __fastcall S_00514aa0_ResizeTo(S_00514aa0* self, void* /*edx*/,
                                                uint32_t count, uint32_t fillValue,
                                                uint32_t /*stackArg3*/, uint32_t /*stackArg4*/, uint32_t /*stackArg5*/)
{
    const uint32_t begin = self->field_0004;
    uint32_t size = 0;
    if (begin != 0) {
        size = static_cast<uint32_t>(static_cast<int32_t>(self->field_0008 - begin) >> 4);
    }

    uint32_t* result = reinterpret_cast<uint32_t*>(count);

    if (size >= count) {
        // Shrinking (or no change) path.
        if (begin != 0) {
            const uint32_t end = self->field_0008;
            const uint32_t endSize = static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> 4);
            if (count < endSize) {
                const uint32_t newEnd = begin + 16 * count;
                result = reinterpret_cast<uint32_t*>(newEnd);
                if (newEnd != end) {
                    result = StlRange_TruncateTo(reinterpret_cast<uint32_t*>(end),
                                                 reinterpret_cast<uint32_t*>(end),
                                                 reinterpret_cast<uint32_t*>(newEnd),
                                                 static_cast<int>(count));
                    self->field_0008 = reinterpret_cast<uint32_t>(result);
                }
            }
        }
        return result;
    }

    // Growing path: current size is recomputed from the live fields.
    uint32_t currentSize = 0;
    if (begin != 0) {
        currentSize = static_cast<uint32_t>(static_cast<int32_t>(self->field_0008 - begin) >> 4);
    }
    return reinterpret_cast<uint32_t*>(
        S_00514aa0_GrowAndFill(self, nullptr,
                               reinterpret_cast<char*>(self->field_0008),
                               count - currentSize,
                               &fillValue));
}

HOOK(0x00516410, S_00514aa0_ResizeTo, "auto");
