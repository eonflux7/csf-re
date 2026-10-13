#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t __cdecl CoreZlibLog_FindLongestMatch(
    hook::Regs* regs, const uint32_t* args)
{
    void* state = reinterpret_cast<void*>(regs->esi);
    uint32_t position = CSF_AT(uint32_t, state, 0x64);
    uint32_t chainRemaining = CSF_AT(uint32_t, state, 0x74);
    uint8_t* scan = CSF_AT(uint8_t*, state, 0x30) + position;
    uint32_t niceLength = CSF_AT(uint32_t, state, 0x88);
    uint32_t windowSize = CSF_AT(uint32_t, state, 0x24);
    uint32_t bestLength = CSF_AT(uint32_t, state, 0x70);

    uint32_t limit = position <= windowSize - 262
        ? 0
        : position - windowSize + 262;

    uint8_t endBefore = scan[bestLength - 1];
    uint8_t endByte = scan[bestLength];

    if (bestLength >= CSF_AT(uint32_t, state, 0x84))
        chainRemaining >>= 2;

    uint32_t lookahead = CSF_AT(uint32_t, state, 0x6C);
    if (niceLength > lookahead)
        niceLength = lookahead;

    uint32_t candidate = args[0];
    do
    {
        uint8_t* match = CSF_AT(uint8_t*, state, 0x30) + candidate;
        if (match[bestLength] == endByte &&
            match[bestLength - 1] == endBefore &&
            match[0] == scan[0] &&
            match[1] == scan[1])
        {
            // The original skips byte 2 and compares through byte 258.
            int32_t matchLength = 2;
            do
            {
                ++matchLength;
            }
            while (scan[matchLength] == match[matchLength] &&
                   matchLength < 258);

            if (matchLength > static_cast<int32_t>(bestLength))
            {
                CSF_AT(uint32_t, state, 0x68) = candidate;
                bestLength = static_cast<uint32_t>(matchLength);
                if (matchLength >= static_cast<int32_t>(niceLength))
                    break;

                endBefore = scan[bestLength - 1];
                endByte = scan[bestLength];
            }
        }

        uint32_t chainIndex = candidate & CSF_AT(uint32_t, state, 0x2C);
        candidate = CSF_AT(uint16_t*, state, 0x38)[chainIndex];
        if (candidate <= limit)
            break;

        --chainRemaining;
    }
    while (chainRemaining != 0);

    return bestLength > lookahead ? lookahead : bestLength;
}

HOOK_REGS(0x006A0940, CoreZlibLog_FindLongestMatch, 0, "core_zlib_log");
