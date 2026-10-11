#include <cstdint>
#include "globals.h"
#include "hook.h"

static void CoreZlibLog_SendBits(void* state, std::uint32_t value, int length)
{
    const int validBits = CSF_AT(int, state, 0x16B4);
    if (validBits <= 16 - length)
    {
        CSF_AT(std::uint16_t, state, 0x16B0) |=
            static_cast<std::uint16_t>(value << validBits);
        CSF_AT(int, state, 0x16B4) = validBits + length;
    }
    else
    {
        auto* output = CSF_AT(std::uint8_t*, state, 0x08);
        CSF_AT(std::uint16_t, state, 0x16B0) |=
            static_cast<std::uint16_t>(value << validBits);

        output[CSF_AT(std::uint32_t, state, 0x14)] =
            CSF_AT(std::uint8_t, state, 0x16B0);

        const std::uint8_t highByte = CSF_AT(std::uint8_t, state, 0x16B1);
        const std::uint32_t nextByte = CSF_AT(std::uint32_t, state, 0x14) + 1;
        CSF_AT(std::uint32_t, state, 0x14) = nextByte;
        CSF_AT(std::uint8_t*, state, 0x08)[nextByte] = highByte;

        const int remainingBits = CSF_AT(int, state, 0x16B4);
        ++CSF_AT(std::uint32_t, state, 0x14);
        CSF_AT(int, state, 0x16B4) = remainingBits + length - 16;
        CSF_AT(std::uint16_t, state, 0x16B0) =
            static_cast<std::uint16_t>(
                static_cast<std::uint16_t>(value) >> (16 - remainingBits));
    }
}

static void CoreZlibLog_SendTreeCode(void* state, int symbol)
{
    const int length = CSF_AT(std::uint16_t, state, 2678 + 4 * symbol);
    const std::uint16_t code = CSF_AT(std::uint16_t, state, 2676 + 4 * symbol);
    CoreZlibLog_SendBits(state, code, length);
}

static std::uintptr_t __cdecl CoreZlibLog_SendCodeLengthTree(
    void* state, const void* tree, int maxCode)
{
    int nextLength = CSF_AT(std::uint16_t, tree, 2);
    int count = 0;
    int previousLength = -1;
    int maxCount = 7;
    int minCount = 4;

    if (!nextLength)
    {
        maxCount = 138;
        minCount = 3;
    }

    if (maxCode >= 0)
    {
        const auto* nextEntry = static_cast<const std::uint8_t*>(tree) + 6;
        std::uint32_t remaining = static_cast<std::uint32_t>(maxCode) + 1;

        do
        {
            const int currentLength = nextLength;
            nextLength = CSF_AT(std::uint16_t, nextEntry, 0);
            ++count;

            if (!(count < maxCount && currentLength == nextLength))
            {
                if (count < minCount)
                {
                    do
                    {
                        CoreZlibLog_SendTreeCode(state, currentLength);
                    }
                    while (--count);
                }
                else if (currentLength)
                {
                    if (currentLength != previousLength)
                    {
                        CoreZlibLog_SendTreeCode(state, currentLength);
                        --count;
                    }

                    CoreZlibLog_SendTreeCode(state, 16);
                    CoreZlibLog_SendBits(
                        state, static_cast<std::uint32_t>(count - 3), 2);
                }
                else if (count <= 10)
                {
                    CoreZlibLog_SendTreeCode(state, 17);
                    CoreZlibLog_SendBits(
                        state, static_cast<std::uint32_t>(count - 3), 3);
                }
                else
                {
                    CoreZlibLog_SendTreeCode(state, 18);
                    CoreZlibLog_SendBits(
                        state, static_cast<std::uint32_t>(count - 11), 7);
                }

                count = 0;
                previousLength = currentLength;

                if (!nextLength)
                {
                    maxCount = 138;
                    minCount = 3;
                }
                else if (currentLength == nextLength)
                {
                    maxCount = 6;
                    minCount = 3;
                }
                else
                {
                    maxCount = 7;
                    minCount = 4;
                }
            }

            nextEntry += 4;
        }
        while (--remaining);
    }

    return reinterpret_cast<std::uintptr_t>(state);
}

HOOK(0x006A3770, CoreZlibLog_SendCodeLengthTree, "auto");
