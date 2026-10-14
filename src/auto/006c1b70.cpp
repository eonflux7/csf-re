#include <cstddef>
#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, TextureDictVector_InsertFilled, 0x006C1910,
         (void* self, void* edx, uint32_t* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

// Resizes the 4-byte-element vector whose begin/end pointers live at self+4 / self+8.
static unsigned int __fastcall ResizeTextureDictVector(
    void* self, void* /*edx*/, unsigned int requestedCount, int fillValue)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x04);
    unsigned int result = 0;

    if (begin != 0) {
        result = static_cast<unsigned int>(
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2);
    }

    if (result >= requestedCount) {
        if (begin != 0) {
            result = CSF_AT(uint32_t, self, 0x08);
            if (requestedCount < static_cast<unsigned int>(
                    static_cast<int32_t>(result - begin) >> 2)) {
                uint32_t newEnd = begin + 4u * requestedCount;
                if (newEnd != result) {
                    // The original computes the copy size as (end - end) >> 2,
                    // which is always zero; memcpy is still called with it.
                    const std::size_t byteCount = 0;
                    void* copied = _memcpy(
                        reinterpret_cast<void*>(newEnd),
                        reinterpret_cast<const void*>(static_cast<uintptr_t>(result)),
                        byteCount);
                    newEnd = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(copied));
                    CSF_AT(uint32_t, self, 0x08) = newEnd;
                    return newEnd;
                }
            }
        }
    } else {
        unsigned int currentCount = 0;
        if (begin != 0) {
            currentCount = static_cast<unsigned int>(
                static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2);
        }
        return static_cast<unsigned int>(TextureDictVector_InsertFilled(
            self, nullptr,
            reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08))),
            requestedCount - currentCount,
            reinterpret_cast<uint32_t*>(&fillValue)));
    }

    return result;
}

HOOK(0x006C1B70, ResizeTextureDictVector, "auto");
