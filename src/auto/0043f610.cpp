#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, Briefing_ProcessEntry, 0x0043F3E0,
         (void* self, void* edx, int a2));

static int __cdecl Briefing_ProcessRecordsBackward(
    int sourceBegin, int sourceEnd, int destinationEnd, int unusedArg)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 196u;
        destination -= 196u;
        Briefing_ProcessEntry(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x0043F610, Briefing_ProcessRecordsBackward, "auto");
