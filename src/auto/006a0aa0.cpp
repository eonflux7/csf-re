#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_UpdateAdler32, 0x006A3200,
         (unsigned int a1, uint8_t* a2, unsigned int a3));

// The original entry receives state in EBX. The hook adapter must marshal
// that register to this argument; this C++ entry itself uses __cdecl.
static unsigned int __cdecl CoreZlibLog_FillWindow(void* state)
{
    const uint32_t windowSize = CSF_AT(uint32_t, state, 0x24);

    for (;;)
    {
        const uint32_t position = CSF_AT(uint32_t, state, 0x64);
        const uint32_t lookahead = CSF_AT(uint32_t, state, 0x6C);
        uint32_t space = CSF_AT(uint32_t, state, 0x34) - position - lookahead;

        if (space == 0 && position == 0 && lookahead == 0)
        {
            space = windowSize;
        }
        else if (space == UINT32_MAX)
        {
            space = UINT32_MAX - 1;
        }
        else if (position >= windowSize + CSF_AT(uint32_t, state, 0x24) - 262)
        {
            uint8_t* window = CSF_AT(uint8_t*, state, 0x30);
            std::memcpy(window, window + windowSize, windowSize);

            const uint32_t matchPosition =
                CSF_AT(uint32_t, state, 0x68) - windowSize;
            const uint32_t newPosition =
                CSF_AT(uint32_t, state, 0x64) - windowSize;
            const uint32_t blockStart =
                CSF_AT(uint32_t, state, 0x54) - windowSize;
            CSF_AT(uint32_t, state, 0x68) = matchPosition;

            uint32_t count = CSF_AT(uint32_t, state, 0x44);
            CSF_AT(uint32_t, state, 0x54) = blockStart;
            uint16_t* head = CSF_AT(uint16_t*, state, 0x3C) + count;
            CSF_AT(uint32_t, state, 0x64) = newPosition;

            do
            {
                const uint32_t entry = *--head;
                --count;
                *head = static_cast<uint16_t>(
                    entry < windowSize ? 0 : entry - windowSize);
            } while (count != 0);

            uint16_t* previous =
                CSF_AT(uint16_t*, state, 0x38) + windowSize;
            count = windowSize;
            do
            {
                const uint32_t entry = *--previous;
                --count;
                *previous = static_cast<uint16_t>(
                    entry < windowSize ? 0 : entry - windowSize);
            } while (count != 0);

            space += windowSize;
        }

        void* stream = CSF_AT(void*, state, 0x00);
        if (CSF_AT(uint32_t, stream, 0x04) == 0)
            break;

        uint8_t* destination =
            CSF_AT(uint8_t*, state, 0x30) +
            CSF_AT(uint32_t, state, 0x64) +
            CSF_AT(uint32_t, state, 0x6C);
        const uint32_t available = CSF_AT(uint32_t, stream, 0x04);
        const uint32_t copied = available > space ? space : available;

        if (copied != 0)
        {
            CSF_AT(uint32_t, stream, 0x04) = available - copied;
            void* streamState = CSF_AT(void*, stream, 0x1C);
            if (CSF_AT(uint32_t, streamState, 0x18) == 0)
            {
                const int checksum = CoreZlibLog_UpdateAdler32(
                    CSF_AT(uint32_t, stream, 0x30),
                    CSF_AT(uint8_t*, stream, 0x00), copied);
                CSF_AT(uint32_t, stream, 0x30) =
                    static_cast<uint32_t>(checksum);
            }

            std::memcpy(destination, CSF_AT(uint8_t*, stream, 0x00), copied);

            uint8_t* nextInput = CSF_AT(uint8_t*, stream, 0x00);
            const uint32_t totalInput = CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint8_t*, stream, 0x00) = nextInput + copied;
            CSF_AT(uint32_t, stream, 0x08) = totalInput + copied;
        }

        const uint32_t newLookahead =
            CSF_AT(uint32_t, state, 0x6C) + copied;
        CSF_AT(uint32_t, state, 0x6C) = newLookahead;

        if (newLookahead >= 3)
        {
            const uint8_t* scan =
                CSF_AT(uint8_t*, state, 0x30) +
                CSF_AT(uint32_t, state, 0x64);
            const uint32_t first = scan[0];
            const uint32_t shift = CSF_AT(uint32_t, state, 0x50);
            CSF_AT(uint32_t, state, 0x40) = first;
            CSF_AT(uint32_t, state, 0x40) =
                ((first << (shift & 31)) ^ scan[1]) &
                CSF_AT(uint32_t, state, 0x4C);
        }

        if (newLookahead >= 0x106 ||
            CSF_AT(uint32_t, CSF_AT(void*, state, 0x00), 0x04) == 0)
            break;
    }

    return windowSize;
}

HOOK(0x006A0AA0, CoreZlibLog_FillWindow, "auto");
