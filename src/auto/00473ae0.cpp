#include <cstddef>
#include "globals.h"
#include <cstdint>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int, __fastcall, DwordVector_InsertN, 0x00473230,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static unsigned int __fastcall DwordVector_Resize(
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
                const uint32_t newEnd = begin + 4u * requestedCount;
                if (newEnd != result) {
                    // The original still calls memcpy with a zero byte count.
                    void* copied = _memcpy(
                        reinterpret_cast<void*>(static_cast<uintptr_t>(newEnd)),
                        reinterpret_cast<const void*>(static_cast<uintptr_t>(result)),
                        0);
                    result = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(copied));
                    CSF_AT(uint32_t, self, 0x08) = result;
                }
            }
        }
    } else {
        unsigned int currentCount = 0;
        if (begin != 0) {
            currentCount = static_cast<unsigned int>(
                static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2);
        }
        return DwordVector_InsertN(
            self, nullptr,
            reinterpret_cast<char*>(static_cast<uintptr_t>(
                CSF_AT(uint32_t, self, 0x08))),
            requestedCount - currentCount,
            reinterpret_cast<uint32_t*>(&fillValue));
    }
    return result;
}

HOOK(0x00473AE0, DwordVector_Resize, "auto");
