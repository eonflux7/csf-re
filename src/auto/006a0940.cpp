#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// The original receives state in ESI, not on the stack. Installing this
// C++ entry point requires an ESI-to-cdecl adapter in the hook layer.
static uint32_t __cdecl CoreZlibLog_FindLongestMatch(
    void* state, uint32_t currentMatch)
{
    const uint32_t position = CSF_AT(uint32_t, state, 0x64);
    uint32_t chainLength = CSF_AT(uint32_t, state, 0x74);
    uint8_t* const scan =
        CSF_AT(uint8_t*, state, 0x30) + position;
    uint32_t niceLength = CSF_AT(uint32_t, state, 0x88);
    const uint32_t windowSize = CSF_AT(uint32_t, state, 0x24);
    uint32_t bestLength = CSF_AT(uint32_t, state, 0x70);

    const uint32_t limit =
        position <= windowSize - 262 ? 0 : position - windowSize + 262;

    uint8_t scanEndPrevious = scan[bestLength - 1];
    uint8_t scanEnd = scan[bestLength];
    uint8_t* const scanLimit = scan + 258;

    if (bestLength >= CSF_AT(uint32_t, state, 0x84))
        chainLength >>= 2;

    const uint32_t lookahead = CSF_AT(uint32_t, state, 0x6C);
    if (niceLength > lookahead)
        niceLength = lookahead;

    do
    {
        uint8_t* const match =
            CSF_AT(uint8_t*, state, 0x30) + currentMatch;

        if (match[bestLength] == scanEnd &&
            match[bestLength - 1] == scanEndPrevious &&
            match[0] == scan[0] &&
            match[1] == scan[1])
        {
            // The original skips byte 2 and checks the boundary only after
            // each group of eight comparisons.
            uint8_t* scanCursor = scan + 2;
            uint8_t* matchCursor = match + 2;
            bool mismatch = false;
            do
            {
                for (int byte = 0; byte < 8; ++byte)
                {
                    ++scanCursor;
                    ++matchCursor;
                    if (*scanCursor != *matchCursor)
                    {
                        mismatch = true;
                        break;
                    }
                }
            }
            while (!mismatch && scanCursor < scanLimit);

            const int32_t length =
                static_cast<int32_t>(scanCursor - scanLimit + 258);

            if (length > static_cast<int32_t>(bestLength))
            {
                CSF_AT(uint32_t, state, 0x68) = currentMatch;
                bestLength = static_cast<uint32_t>(length);
                if (length >= static_cast<int32_t>(niceLength))
                    break;

                scanEndPrevious = scan[length - 1];
                scanEnd = scan[length];
            }
        }

        const uint32_t chainIndex =
            currentMatch & CSF_AT(uint32_t, state, 0x2C);
        currentMatch = CSF_AT(uint16_t*, state, 0x38)[chainIndex];
        if (currentMatch <= limit)
            break;
        --chainLength;
    }
    while (chainLength != 0);

    return bestLength > lookahead ? lookahead : bestLength;
}

HOOK(0x006A0940, CoreZlibLog_FindLongestMatch, "auto");
