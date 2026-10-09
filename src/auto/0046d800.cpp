#include <cstddef>
#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static unsigned int __fastcall ResizeIntVectorToCount(
    S_0046c410* self, void* /*edx*/, unsigned int requestedCount, int fillValue)
{
    const uint32_t begin = self->field_0004;
    unsigned int result = 0;

    if (begin != 0) {
        result = static_cast<unsigned int>(
            static_cast<int32_t>(self->field_0008 - begin) >> 2);
    }

    if (result >= requestedCount) {
        if (begin != 0) {
            result = self->field_0008;
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
                    self->field_0008 = newEnd;
                    return newEnd;
                }
            }
        }
    } else {
        unsigned int currentCount = 0;
        if (begin != 0) {
            currentCount = static_cast<unsigned int>(
                static_cast<int32_t>(self->field_0008 - begin) >> 2);
        }
        return IntVector_InsertN(
            self, nullptr,
            reinterpret_cast<char*>(static_cast<uintptr_t>(self->field_0008)),
            requestedCount - currentCount,
            reinterpret_cast<uint32_t*>(&fillValue));
    }

    return result;
}

HOOK(0x0046D800, ResizeIntVectorToCount, "auto");
