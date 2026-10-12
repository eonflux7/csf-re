#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, sub_69B1C0, 0x0069B1C0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

// The packet also lists the primary handler's address as a vtable reference.
static const auto g_PrimaryHandlerReference = CSF_VTBL(0x0069B1C0);

static char __fastcall S_0069b1c0_TryReleaseBlock(
    S_0069b1c0* self, void* /*edx*/, unsigned int address)
{
    char result = sub_69B1C0(self, nullptr, address);
    if (result)
        return result;

    const std::uint32_t initialCount = self->field_0034;
    if (!initialCount)
        return result;

    void* block;
    std::uint32_t attempts = 0;
    for (;;)
    {
        const std::int32_t cursor =
            static_cast<std::int32_t>(self->field_003c);
        if (cursor >= static_cast<std::int32_t>(initialCount) || cursor < 0)
            self->field_003c = 0;

        const std::uint32_t index = self->field_003c;
        self->field_003c = index + 1;
        block = reinterpret_cast<void**>(self->field_002c)[index];

        if (CSF_AT(std::uint8_t, block, 0x00) &&
            address >= CSF_AT(std::uint32_t, block, 0x04) &&
            address < CSF_AT(std::uint32_t, block, 0x0C))
        {
            break;
        }

        if (++attempts >= self->field_0034)
            return result;
    }

    const std::uint32_t slot =
        (address - CSF_AT(std::uint32_t, block, 0x08)) /
        CSF_AT(std::uint32_t, block, 0x10);
    const std::uint32_t bitmapIndex = slot >> 3;
    auto* bitmap = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, block, 0x04));
    bitmap[bitmapIndex] &= static_cast<std::uint8_t>(~(1u << (slot & 7)));

    const std::uint32_t remaining =
        CSF_AT(std::uint32_t, block, 0x1C) - 1;
    CSF_AT(std::uint32_t, block, 0x18) = bitmapIndex;
    CSF_AT(std::uint32_t, block, 0x1C) = remaining;
    --self->field_003c;

    if (CSF_AT(std::uint32_t, block, 0x1C))
        return 1;

    for (std::uint32_t index = 0; index < self->field_0034; ++index)
    {
        if (reinterpret_cast<void**>(self->field_002c)[index] != block)
            continue;

        const std::uint32_t newCount = self->field_0034 - 1;
        self->field_0034 = newCount;
        if (index != newCount)
        {
            auto* destination =
                reinterpret_cast<std::uint8_t*>(self->field_002c) + 4 * index;
            _memcpy(destination, destination + 4, 4 * (newCount - index));
        }

        if (block)
        {
            if (CSF_AT(std::uint8_t, block, 0x00))
            {
                if (CSF_AT(std::uint8_t, block, 0x28))
                {
                    _free(reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, block, 0x04)));
                }
                else if (CSF_AT(std::uint32_t, block, 0x04))
                {
                    Mem_Free(reinterpret_cast<void*>(
                        CSF_AT(std::uint32_t, block, 0x04)));
                    CSF_AT(std::uint32_t, block, 0x04) = 0;
                }
            }

            Mem_Free(block);
            block = nullptr;
        }
    }

    if (!self->field_0034)
    {
        void* blocks = reinterpret_cast<void*>(self->field_002c);
        CSF_AT(std::uint32_t, self, 0x30) = 0;
        self->field_0034 = 0;
        if (blocks)
        {
            Mem_Free(blocks);
            self->field_002c = 0;
        }
    }

    return 1;
}

HOOK(0x0069B590, S_0069b1c0_TryReleaseBlock, "auto");
