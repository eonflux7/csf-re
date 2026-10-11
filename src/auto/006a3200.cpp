#include <cstdint>
#include "hook.h"

static int __cdecl CoreZlibLog_ComputeAdler32(
    unsigned int checksum, unsigned char* data, unsigned int length)
{
    std::uint32_t lowSum = static_cast<std::uint16_t>(checksum);
    std::uint32_t highSum = checksum >> 16;

    if (!data)
        return 1;

    while (length)
    {
        unsigned int chunkLength = length;
        if (length >= 0x15B0)
            chunkLength = 5552;
        length -= chunkLength;

        while (chunkLength >= 16)
        {
            for (unsigned int index = 0; index < 16; ++index)
            {
                lowSum += data[index];
                highSum += lowSum;
            }
            data += 16;
            chunkLength -= 16;
        }

        while (chunkLength)
        {
            lowSum += *data++;
            highSum += lowSum;
            --chunkLength;
        }

        lowSum %= 0xFFF1u;
        highSum %= 0xFFF1u;
    }

    return static_cast<int>(lowSum | (highSum << 16));
}

HOOK(0x006A3200, CoreZlibLog_ComputeAdler32, "auto");
