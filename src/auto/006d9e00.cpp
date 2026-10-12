#include <cstdint>
#include "hook.h"

// Copies 16-byte records (four dwords each) from [srcBegin, srcEnd) to dst.
// Returns the destination pointer just past the last record written.
// The fourth argument (ebp+0x14) is never read by the original.
static uint32_t* __cdecl ProfilesSavegames_AssignRecordRange(uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dst, int /*unused*/)
{
    uint32_t* result = dst;
    for (uint32_t* src = srcBegin; src != srcEnd; src += 4, result += 4)
    {
        result[0] = src[0];
        result[1] = src[1];
        result[2] = src[2];
        uint32_t lastWord = src[3];
        result[3] = lastWord;
    }
    return result;
}

HOOK(0x6D9E00, ProfilesSavegames_AssignRecordRange, "auto");
