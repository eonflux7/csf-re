#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_ComputeAdler32, 0x006A3200, (unsigned int checksum, unsigned char* data, unsigned int length));

static uint32_t __cdecl CoreZlibLog_FillWindow(
    hook::Regs* regs, const uint32_t* /*args*/)
{
    void* state = reinterpret_cast<void*>(regs->ebx);
    const uint32_t windowSize = CSF_AT(uint32_t, state, 0x24);

    uint32_t lookahead;
    do
    {
        const uint32_t position = CSF_AT(uint32_t, state, 0x64);
        const uint32_t buffered = CSF_AT(uint32_t, state, 0x6C);
        uint32_t space = CSF_AT(uint32_t, state, 0x34) - position - buffered;

        if (space == 0 && position == 0 && buffered == 0)
        {
            space = windowSize;
        }
        else if (space == UINT32_MAX)
        {
            space = UINT32_MAX - 1;
        }
        else if (position >= windowSize + CSF_AT(uint32_t, state, 0x24) - 262)
        {
            std::memcpy(
                CSF_AT(uint8_t*, state, 0x30),
                CSF_AT(uint8_t*, state, 0x30) + windowSize,
                windowSize);

            const uint32_t newPosition = CSF_AT(uint32_t, state, 0x64) - windowSize;
            const uint32_t newMatchStart = CSF_AT(uint32_t, state, 0x54) - windowSize;
            CSF_AT(uint32_t, state, 0x68) -= windowSize;
            uint32_t headCount = CSF_AT(uint32_t, state, 0x44);
            CSF_AT(uint32_t, state, 0x54) = newMatchStart;
            uint16_t* headEnd = CSF_AT(uint16_t*, state, 0x3C) + headCount;
            CSF_AT(uint32_t, state, 0x64) = newPosition;

            do
            {
                const uint32_t link = *--headEnd;
                --headCount;
                *headEnd = static_cast<uint16_t>(link < windowSize ? 0 : link - windowSize);
            }
            while (headCount);

            uint32_t previousCount = windowSize;
            uint16_t* previousEnd = CSF_AT(uint16_t*, state, 0x38) + windowSize;
            do
            {
                const uint32_t link = *--previousEnd;
                --previousCount;
                *previousEnd = static_cast<uint16_t>(link < windowSize ? 0 : link - windowSize);
            }
            while (previousCount);

            space += windowSize;
        }

        void* stream = CSF_AT(void*, state, 0);
        if (CSF_AT(uint32_t, stream, 0x04) == 0)
            break;

        uint8_t* destination = CSF_AT(uint8_t*, state, 0x30)
            + CSF_AT(uint32_t, state, 0x64)
            + CSF_AT(uint32_t, state, 0x6C);
        const uint32_t available = CSF_AT(uint32_t, stream, 0x04);
        const uint32_t count = available > space ? space : available;

        if (count != 0)
        {
            CSF_AT(uint32_t, stream, 0x04) = available - count;
            if (CSF_AT(uint32_t, CSF_AT(void*, stream, 0x1C), 0x18) == 0)
            {
                const int checksum = CoreZlibLog_ComputeAdler32(
                    CSF_AT(uint32_t, stream, 0x30),
                    CSF_AT(unsigned char*, stream, 0),
                    count);
                CSF_AT(uint32_t, stream, 0x30) = static_cast<uint32_t>(checksum);
            }

            std::memcpy(destination, CSF_AT(uint8_t*, stream, 0), count);
            const uint32_t totalInput = CSF_AT(uint32_t, stream, 0x08);
            CSF_AT(uint8_t*, stream, 0) += count;
            CSF_AT(uint32_t, stream, 0x08) = totalInput + count;
        }

        lookahead = count + CSF_AT(uint32_t, state, 0x6C);
        CSF_AT(uint32_t, state, 0x6C) = lookahead;
        if (lookahead >= 3)
        {
            const uint8_t* start = CSF_AT(uint8_t*, state, 0x30)
                + CSF_AT(uint32_t, state, 0x64);
            const uint32_t firstByte = start[0];
            const uint32_t hashShift = CSF_AT(uint32_t, state, 0x50);
            CSF_AT(uint32_t, state, 0x40) = firstByte;
            CSF_AT(uint32_t, state, 0x40) =
                CSF_AT(uint32_t, state, 0x4C)
                & (start[1] ^ (firstByte << hashShift));
        }
    }
    while (lookahead < 0x106
        && CSF_AT(uint32_t, CSF_AT(void*, state, 0), 0x04));

    return windowSize;
}

HOOK_REGS(0x006A0AA0, CoreZlibLog_FillWindow, 0, "core_zlib_log");
