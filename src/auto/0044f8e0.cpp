#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall S_0044f8e0_GrowCapacity(
    S_0044f8e0* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    const int allocationResult = CoreMemory_AllocUntagged(16 * capacity);
    auto* replacement = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(allocationResult)));

    if (replacement && static_cast<std::int32_t>(capacity - 1) >= 0)
    {
        auto* entry = replacement;
        auto* flags = replacement + 14;
        std::uint32_t remaining = capacity;
        do
        {
            flags[-1] = 0;
            flags[0] = 0;
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            flags[-2] = 0;
            entry += 16;
            flags += 16;
            --remaining;
        } while (remaining);
    }

    const std::uint32_t count = self->field_0008;
    if (count)
    {
        std::memcpy(replacement,
                    reinterpret_cast<const void*>(self->field_0000),
                    16 * count);
    }

    void* oldStorage = reinterpret_cast<void*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    int result = allocationResult;
    if (oldStorage)
    {
        result = CoreMemory_Free(oldStorage);
        self->field_0000 = 0;
    }

    self->field_0000 = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(replacement));
    return result;
}

HOOK(0x0044F8E0, S_0044f8e0_GrowCapacity, "auto");
