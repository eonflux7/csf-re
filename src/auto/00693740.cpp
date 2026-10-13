#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

using PackedBufferPoolSlots = std::uint32_t[16];
CSF_GLOBAL(PackedBufferPoolSlots, g_ComponentPools, 0x008BAF48);
CSF_GLOBAL(std::uint32_t, g_DefaultRecordValue, 0x008A66B0);
CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

// The packet labels the direct array-growth call as a vtable reference.
[[maybe_unused]] static const auto g_ArrayGrowthReference = CSF_VTBL(0x0041E5F0);

static char __fastcall S_00693740_OpenPackedBuffer(
    S_00693740* self, void* /*edx*/)
{
    auto* header = reinterpret_cast<std::uint32_t*>(self->field_0004);
    self->field_0015 = 0xFF;

    for (std::uint32_t slot = 0; slot < 0x10; ++slot)
    {
        if (!g_ComponentPools[slot])
        {
            g_ComponentPools[slot] = reinterpret_cast<std::uint32_t>(self);
            self->field_0015 = static_cast<std::uint8_t>(slot);
            break;
        }
    }

    if (self->field_0015 == 0xFF)
        return 0;

    const std::uint32_t format = header[2];
    const std::uint32_t recordCount = header[3];
    const std::uint32_t firstBlockCount = header[4];
    const std::uint32_t secondBlockCount = header[5];
    CSF_AT(std::uint32_t, self, 0x18) = recordCount;

    std::uint32_t cursor = reinterpret_cast<std::uint32_t>(header + 6);
    if (format != g_DefaultRecordValue)
        return 0;

    self->field_0000 = 0;
    if (recordCount)
    {
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(cursor), 0x0B) =
            self->field_0015;
        self->field_0000 = cursor;
    }

    cursor += 12 * CSF_AT(std::uint32_t, self, 0x18);

    for (std::uint32_t remaining = firstBlockCount; remaining; --remaining)
    {
        const std::uint32_t length =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(cursor), 0);
        cursor += 4;

        if (length)
        {
            if (self->field_0024 >= CSF_AT(std::uint32_t, self, 0x20))
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        reinterpret_cast<std::uint8_t*>(self) + 0x1C),
                    nullptr, CSF_AT(int, self, 0x28));
            }

            const std::uint32_t index = self->field_0024;
            auto* destination = reinterpret_cast<std::uint32_t*>(
                self->field_001c + 4 * index);
            self->field_0024 = index + 1;
            *destination = cursor;
            cursor += length;
        }
    }

    for (std::uint32_t remaining = secondBlockCount; remaining; --remaining)
    {
        const std::uint32_t length =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(cursor), 0);
        const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x30);
        cursor += 4;

        if (self->field_0034 >= capacity)
        {
            const std::uint32_t growth = CSF_AT(std::uint32_t, self, 0x38);
            void* replacement = reinterpret_cast<void*>(
                CoreMemory_AllocUntagged(4 * (growth + capacity)));

            const std::uint32_t count = self->field_0034;
            if (count)
            {
                std::memcpy(replacement,
                            reinterpret_cast<const void*>(self->field_002c),
                            4 * count);
            }

            CSF_AT(std::uint32_t, self, 0x30) += growth;
            if (self->field_002c)
            {
                CoreMemory_Free(reinterpret_cast<void*>(self->field_002c));
                self->field_002c = 0;
            }
            self->field_002c = reinterpret_cast<std::uint32_t>(replacement);
        }

        const std::uint32_t index = self->field_0034;
        auto* destination = reinterpret_cast<std::uint32_t*>(
            self->field_002c + 4 * index);
        self->field_0034 = index + 1;

        if (length)
        {
            *destination = cursor;
            cursor += length;
        }
        else
        {
            *destination = reinterpret_cast<std::uint32_t>(&g_byte_82E65C);
        }
    }

    return 1;
}

HOOK(0x00693740, S_00693740_OpenPackedBuffer, "auto");
