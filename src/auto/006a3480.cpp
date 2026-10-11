#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __cdecl CoreZlibLog_GenerateBitLengths(
    std::uint32_t* state, void* descriptor)
{
    auto* tree = CSF_AT(std::uint8_t*, descriptor, 0x00);
    const std::int32_t maxCode = CSF_AT(std::int32_t, descriptor, 0x04);
    void* staticDescriptor = CSF_AT(void*, descriptor, 0x08);
    auto* staticTree = CSF_AT(std::uint8_t*, staticDescriptor, 0x00);
    auto* extraBits = CSF_AT(std::uint8_t*, staticDescriptor, 0x04);
    const std::int32_t extraBase = CSF_AT(std::int32_t, staticDescriptor, 0x08);
    const std::int32_t maxLength = CSF_AT(std::int32_t, staticDescriptor, 0x10);

    CSF_AT(std::uint32_t, state, 0xB34) = 0;
    CSF_AT(std::uint32_t, state, 0xB38) = 0;
    CSF_AT(std::uint32_t, state, 0xB3C) = 0;
    CSF_AT(std::uint32_t, state, 0xB40) = 0;
    CSF_AT(std::uint32_t, state, 0xB44) = 0;
    CSF_AT(std::uint32_t, state, 0xB48) = 0;
    CSF_AT(std::uint32_t, state, 0xB4C) = 0;
    CSF_AT(std::uint32_t, state, 0xB50) = 0;

    auto* heap = reinterpret_cast<std::uint8_t*>(state) + 0xB54;
    const std::int32_t root = CSF_AT(
        std::int32_t, heap, 4 * CSF_AT(std::int32_t, state, 0x144C));
    CSF_AT(std::uint16_t, tree, 4 * root + 2) = 0;

    std::int32_t heapIndex = CSF_AT(std::int32_t, state, 0x144C) + 1;
    if (heapIndex >= 573)
        return state;

    std::int32_t overflow = 0;
    for (; heapIndex < 573; ++heapIndex)
    {
        const std::int32_t node = CSF_AT(std::int32_t, heap, 4 * heapIndex);
        const std::uint16_t parent = CSF_AT(std::uint16_t, tree, 4 * node + 2);
        std::int32_t length = CSF_AT(std::uint16_t, tree, 4 * parent + 2) + 1;
        if (length > maxLength)
        {
            ++overflow;
            length = maxLength;
        }

        CSF_AT(std::uint16_t, tree, 4 * node + 2) =
            static_cast<std::uint16_t>(length);
        if (node <= maxCode)
        {
            ++CSF_AT(std::uint16_t, state, 0xB34 + 2 * length);
            std::int32_t extra = 0;
            if (node >= extraBase)
                extra = CSF_AT(std::int32_t, extraBits, 4 * (node - extraBase));

            const std::uint32_t frequency = CSF_AT(std::uint16_t, tree, 4 * node);
            CSF_AT(std::uint32_t, state, 0x16A0) +=
                frequency * (static_cast<std::uint32_t>(extra) +
                             static_cast<std::uint32_t>(length));
            if (staticTree)
            {
                const std::uint32_t staticLength =
                    CSF_AT(std::uint16_t, staticTree, 4 * node + 2);
                CSF_AT(std::uint32_t, state, 0x16A4) +=
                    frequency * (static_cast<std::uint32_t>(extra) + staticLength);
            }
        }
    }

    if (!overflow)
        return state;

    do
    {
        std::int32_t length = maxLength - 1;
        while (!CSF_AT(std::uint16_t, state, 0xB34 + 2 * length))
            --length;
        --CSF_AT(std::uint16_t, state, 0xB34 + 2 * length);
        CSF_AT(std::uint16_t, state, 0xB36 + 2 * length) += 2;
        --CSF_AT(std::uint16_t, state, 0xB34 + 2 * maxLength);
        overflow -= 2;
    } while (overflow > 0);

    heapIndex = 573;
    for (std::int32_t length = maxLength; length != 0; --length)
    {
        std::int32_t remaining = CSF_AT(std::uint16_t, state, 0xB34 + 2 * length);
        while (remaining)
        {
            --heapIndex;
            const std::int32_t node = CSF_AT(std::int32_t, heap, 4 * heapIndex);
            if (node > maxCode)
                continue;

            const std::int32_t oldLength = CSF_AT(std::uint16_t, tree, 4 * node + 2);
            if (oldLength != length)
            {
                const std::uint32_t frequency = CSF_AT(std::uint16_t, tree, 4 * node);
                CSF_AT(std::uint32_t, state, 0x16A0) +=
                    frequency * static_cast<std::uint32_t>(length - oldLength);
                CSF_AT(std::uint16_t, tree, 4 * node + 2) =
                    static_cast<std::uint16_t>(length);
            }
            --remaining;
        }
    }
    return state;
}

HOOK(0x006A3480, CoreZlibLog_GenerateBitLengths, "auto");
