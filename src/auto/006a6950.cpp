#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using InflateMasks = uint32_t[17];
CSF_GLOBAL(InflateMasks, g_InflateMasks, 0x008A7D28);

static int __cdecl CoreZlibLog_InflateFast(
    int literalRootBits,
    int distanceRootBits,
    const uint8_t* literalTable,
    const uint8_t* distanceTable,
    void* state,
    void* stream)
{
    uint8_t* input = CSF_AT(uint8_t*, stream, 0x00);
    uint32_t available = CSF_AT(uint32_t, stream, 0x04);
    uint32_t bitCount = CSF_AT(uint32_t, state, 0x1C);
    uint32_t bits = CSF_AT(uint32_t, state, 0x20);
    uint8_t* output = CSF_AT(uint8_t*, state, 0x34);
    const uintptr_t readPosition =
        reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, state, 0x30));

    uint32_t space;
    if (reinterpret_cast<uintptr_t>(output) >= readPosition)
    {
        space = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, state, 0x2C)) -
            reinterpret_cast<uintptr_t>(output));
    }
    else
    {
        space = static_cast<uint32_t>(
            readPosition - reinterpret_cast<uintptr_t>(output) - 1);
    }

    const uint32_t literalMask = g_InflateMasks[literalRootBits];
    const uint32_t distanceMask = g_InflateMasks[distanceRootBits];
    int result = 0;

    for (;;)
    {
        while (bitCount < 0x14)
        {
            --available;
            bits |= static_cast<uint32_t>(*input++) << bitCount;
            bitCount += 8;
        }

        const uint8_t* entry = literalTable + 8 * (bits & literalMask);
        uint32_t operation;

        for (;;)
        {
            operation = CSF_AT(uint8_t, entry, 0x00);
            const uint32_t consumed = CSF_AT(uint8_t, entry, 0x01);
            bits >>= consumed;
            bitCount -= consumed;

            if (operation == 0)
            {
                *output++ = CSF_AT(uint8_t, entry, 0x04);
                --space;
                break;
            }

            if (operation & 0x10)
                break;

            if (operation & 0x40)
            {
                if (operation & 0x20)
                {
                    result = 1;
                }
                else
                {
                    CSF_AT(const char*, stream, 0x18) =
                        CSF_STR(0x0085F9D8, "invalid literal/length code");
                    result = -3;
                }
                goto Finish;
            }

            const uint32_t index =
                CSF_AT(uint32_t, entry, 0x04) +
                (bits & g_InflateMasks[operation]);
            entry += 8 * index;
        }

        if (operation != 0)
        {
            const uint32_t lengthExtra = operation & 0xF;
            const uint32_t length =
                CSF_AT(uint32_t, entry, 0x04) +
                (bits & g_InflateMasks[lengthExtra]);
            bits >>= lengthExtra;
            bitCount -= lengthExtra;

            while (bitCount < 0xF)
            {
                --available;
                bits |= static_cast<uint32_t>(*input++) << bitCount;
                bitCount += 8;
            }

            entry = distanceTable + 8 * (bits & distanceMask);
            for (;;)
            {
                operation = CSF_AT(uint8_t, entry, 0x00);
                const uint32_t consumed = CSF_AT(uint8_t, entry, 0x01);
                bits >>= consumed;
                bitCount -= consumed;

                if (operation & 0x10)
                    break;

                if (operation & 0x40)
                {
                    CSF_AT(const char*, stream, 0x18) =
                        CSF_STR(0x0085F9C0, "invalid distance code");
                    result = -3;
                    goto Finish;
                }

                const uint32_t index =
                    CSF_AT(uint32_t, entry, 0x04) +
                    (bits & g_InflateMasks[operation]);
                entry += 8 * index;
            }

            const uint32_t distanceExtra = operation & 0xF;
            while (bitCount < distanceExtra)
            {
                --available;
                bits |= static_cast<uint32_t>(*input++) << bitCount;
                bitCount += 8;
            }

            const uint32_t distance =
                CSF_AT(uint32_t, entry, 0x04) +
                (bits & g_InflateMasks[distanceExtra]);
            bitCount -= distanceExtra;
            bits >>= distanceExtra;
            space -= length;

            uintptr_t sourceAddress =
                reinterpret_cast<uintptr_t>(output) - distance;
            const uintptr_t windowStart =
                reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, state, 0x28));
            bool crossesEnd = false;
            uint32_t firstPart = 0;

            if (sourceAddress < windowStart)
            {
                const uintptr_t windowEnd =
                    reinterpret_cast<uintptr_t>(
                        CSF_AT(uint8_t*, state, 0x2C));
                do
                {
                    sourceAddress += windowEnd - windowStart;
                }
                while (sourceAddress < windowStart);

                firstPart = static_cast<uint32_t>(windowEnd - sourceAddress);
                crossesEnd = length > firstPart;
            }

            uint8_t* source = reinterpret_cast<uint8_t*>(sourceAddress);
            if (!crossesEnd)
            {
                *output++ = *source++;
                *output++ = *source++;
                uint32_t remaining = length - 2;
                do
                {
                    *output++ = *source++;
                    --remaining;
                }
                while (remaining);
            }
            else
            {
                uint32_t remaining = length - firstPart;
                do
                {
                    *output++ = *source++;
                    --firstPart;
                }
                while (firstPart);

                source = CSF_AT(uint8_t*, state, 0x28);
                do
                {
                    *output++ = *source++;
                    --remaining;
                }
                while (remaining);
            }
        }

        if (space < 0x102 || available < 0xA)
            goto Finish;
    }

Finish:
    uint32_t unread = CSF_AT(uint32_t, stream, 0x04) - available;
    if ((bitCount >> 3) < unread)
        unread = bitCount >> 3;

    CSF_AT(uint32_t, state, 0x20) = bits;
    CSF_AT(uint32_t, state, 0x1C) = bitCount - 8 * unread;

    uint8_t* originalInput = CSF_AT(uint8_t*, stream, 0x00);
    CSF_AT(uint32_t, stream, 0x04) = available + unread;
    input -= unread;
    const uint32_t totalInput =
        CSF_AT(uint32_t, stream, 0x08) +
        static_cast<uint32_t>(input - originalInput);
    CSF_AT(uint8_t*, stream, 0x00) = input;
    CSF_AT(uint32_t, stream, 0x08) = totalInput;
    CSF_AT(uint8_t*, state, 0x34) = output;
    return result;
}

HOOK(0x006A6950, CoreZlibLog_InflateFast, "auto");
