#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ProcessTableEntry, 0x0051A020,
         (void* self, void* edx, int a2));

static S_0046c410* __cdecl BddTables_ProcessEntryRange(int sourceBegin, int sourceEnd,
                                                       S_0046c410* destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = reinterpret_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        ProcessTableEntry(reinterpret_cast<void*>(destination),
                          nullptr,
                          static_cast<int>(source));
        source += 16u;
        destination += 16u;
    }
    while (source != end);

    return reinterpret_cast<S_0046c410*>(destination);
}

HOOK(0x0051A180, BddTables_ProcessEntryRange, "auto");
