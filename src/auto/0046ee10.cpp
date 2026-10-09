#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60,
         (void* self, void* edx, int a2));

static int __cdecl MissionScript_ProcessPairsBackward(
    int sourceBegin, int sourceEnd, int destinationEnd, int unusedArg)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 12u;
        destination -= 12u;
        ProcessEntryPair(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x0046EE10, MissionScript_ProcessPairsBackward, "auto");
