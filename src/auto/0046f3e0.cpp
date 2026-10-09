#include <cstdint>
#include "hook.h"

// Sets (flag != 0) or clears (flag == 0) the bit at (startWord, startBit) and every
// following bit up to (endWord, endBit), walking 32-bit words forward.
static int* __cdecl MissionScript_FillBitRange(uint32_t startBit, int* startWord, int endBit, int* endWord, uint8_t* setFlag)
{
    int* word = startWord;
    uint32_t bit = startBit;

    while (word != endWord || bit != static_cast<uint32_t>(endBit))
    {
        int value;
        if (*setFlag)
            value = static_cast<int>((1u << bit) | static_cast<uint32_t>(*word));
        else
            value = static_cast<int>(~(1u << bit) & static_cast<uint32_t>(*word));
        *word = value;

        if (bit >= 0x1F)
        {
            bit = 0;
            ++word;
        }
        else
        {
            ++bit;
        }
    }
    return word;
}

HOOK(0x46F3E0, MissionScript_FillBitRange, "auto");
