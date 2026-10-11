#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GrowPairBuffer(void* self, void* /*edx*/, int extraCount)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) + static_cast<std::uint32_t>(extraCount);

    auto* newBuffer = static_cast<std::int32_t*>(Mem_Alloc(8 * newCapacity));
    if (newBuffer)
    {
        auto* cursor = newBuffer;
        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            std::uint32_t remaining = newCapacity;
            do
            {
                cursor[1] = -1;
                cursor[0] = -1;
                cursor += 2;
                --remaining;
            } while (remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00),
                    static_cast<std::size_t>(8 * usedCount));
    }

    const void* result = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) += static_cast<std::uint32_t>(extraCount);
    if (result)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(const_cast<void*>(result))));
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    CSF_AT(void*, self, 0x00) = newBuffer;
    return result;
}

HOOK(0x00732DA0, GrowPairBuffer, "auto");
