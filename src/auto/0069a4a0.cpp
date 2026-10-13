#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall C_0085e7b8_WriteBytes(
    C_0085e7b8* self, void* /*edx*/, const void* source, int count)
{
    const std::uint32_t capacity = self->field_0010;
    const int available = static_cast<std::int32_t>(
        capacity - self->field_0008);

    if (count > available)
    {
        const std::uint32_t growth =
            (((static_cast<std::uint32_t>(count) -
               static_cast<std::uint32_t>(available)) >> 8) + 1) << 8;

        void* newBuffer = reinterpret_cast<void*>(
            static_cast<std::uint32_t>(
                CoreMemory_AllocUntagged(capacity + growth)));

        if (newBuffer)
        {
            std::memcpy(newBuffer,
                        reinterpret_cast<const void*>(self->field_0004),
                        self->field_0010);

            if (self->field_0004)
            {
                CoreMemory_Free(reinterpret_cast<void*>(self->field_0004));
                self->field_0004 = 0;
            }

            const std::uint32_t currentCapacity = self->field_0010;
            self->field_0004 = reinterpret_cast<std::uintptr_t>(newBuffer);
            self->field_0010 = currentCapacity + growth;
        }
        else
        {
            count = available;
        }
    }

    std::memcpy(
        reinterpret_cast<std::uint8_t*>(self->field_0004) + self->field_0008,
        source, static_cast<std::size_t>(count));

    const std::uint32_t newPosition =
        self->field_0008 + static_cast<std::uint32_t>(count);
    const std::uint32_t previousLength = self->field_000c;
    self->field_0008 = newPosition;
    if (static_cast<std::int32_t>(newPosition) >
        static_cast<std::int32_t>(previousLength))
    {
        self->field_000c = newPosition;
    }

    return count;
}

HOOK(0x0069A4A0, C_0085e7b8_WriteBytes, "auto");
