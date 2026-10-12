#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl GfxScreenEffects_FillRecordRange(
    std::uint32_t* first, std::uint32_t* last, std::uint32_t* source)
{
    for (std::uint32_t* record = first; record != last; record += 11)
    {
        record[0] = source[0];
        AssignStringSubstring(record + 1, nullptr, source + 1, 0, -1);
        record[8] = source[8];
        record[9] = source[9];
        record[10] = source[10];
    }
    return last;
}

HOOK(0x006F88F0, GfxScreenEffects_FillRecordRange, "auto");
