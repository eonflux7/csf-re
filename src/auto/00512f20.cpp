#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SaveSlotInfo_AssignFromRecord, 0x00512D80,
         (void* self, void* edx, uint32_t record));

static int __cdecl SaveSlotInfo_CopyRangeBackward(
    int sourceBegin, int sourceEnd, int destinationEnd, int unusedArg)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 200u;
        destination -= 200u;
        SaveSlotInfo_AssignFromRecord(
            reinterpret_cast<void*>(destination),
            nullptr,
            source);
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x00512F20, SaveSlotInfo_CopyRangeBackward, "auto");
