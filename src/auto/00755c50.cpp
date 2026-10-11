#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: AssignStringSubstring (0x4109E0), __thiscall, ret 12.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a backward-indexed range of 0x50-byte records from [begin, end) into the
// block ending at destEnd, walking from the end toward begin. Returns the new
// front of the destination block (or destEnd when the range is empty).
static std::uint32_t* __cdecl GameConfig_CopyRecordRangeBackward(std::uint32_t* begin,
                                                                 std::uint32_t* end,
                                                                 std::uint32_t* destEnd,
                                                                 int /*unused*/)
{
    std::uint32_t* src = end;
    if (begin == end)
        return destEnd;

    std::uint32_t* dst = destEnd;
    do
    {
        std::uint32_t firstWord = *(src - 20);
        src -= 20;
        dst -= 20;
        *dst = firstWord;

        AssignStringSubstring(dst + 1, nullptr, src + 1, 0, 0xFFFFFFFFu);

        dst[8]  = src[8];
        dst[9]  = src[9];
        dst[10] = src[10];
        dst[11] = src[11];

        AssignStringSubstring(dst + 12, nullptr, src + 12, 0, 0xFFFFFFFFu);

        dst[19] = src[19];
    }
    while (src != begin);

    return dst;
}

HOOK(0x755C50, GameConfig_CopyRecordRangeBackward, "auto");
