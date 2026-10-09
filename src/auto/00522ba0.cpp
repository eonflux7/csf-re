#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall S_00522ba0_GrowCapacity(
    S_00522ba0* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    void* replacement = Mem_Alloc(
        static_cast<std::uint32_t>(12 * newCapacity));

    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(replacement);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(float, entry, 0x8) = -1.0f;
            entry += 12;
        }
        while (--remaining);
    }

    const std::uint32_t count = self->field_0008;
    if (count)
    {
        std::memcpy(
            replacement,
            reinterpret_cast<const void*>(self->field_0000),
            static_cast<std::uint32_t>(12 * count));
    }

    void* oldBuffer = reinterpret_cast<void*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    int result = 0;
    if (oldBuffer)
    {
        result = Mem_Free(oldBuffer);
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<std::uint32_t>(replacement);
    return result;
}

HOOK(0x00522BA0, S_00522ba0_GrowCapacity, "auto");
