#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60,
         (void* self, void* edx, int a2));

static int __cdecl MissionScript_ProcessEntryRange(int sourceBegin, int sourceEnd,
                                                   int destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination =
        static_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        ProcessEntryPair(reinterpret_cast<void*>(destination),
                         nullptr,
                         static_cast<int>(source));
        source += 12u;
        destination += 12u;
    }
    while (source != end);

    return static_cast<int>(destination);
}

HOOK(0x0046EDD0, MissionScript_ProcessEntryRange, "auto");
