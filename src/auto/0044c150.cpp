#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, ProcessFixedRecord, 0x0044C0C0,
         (void* self, void* edx, int a2));

static char* __cdecl FixedRecords_ProcessRange(int sourceBegin, int sourceEnd,
                                               char* destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination =
        reinterpret_cast<std::uintptr_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        ProcessFixedRecord(reinterpret_cast<void*>(destination),
                           nullptr,
                           static_cast<int>(source));
        source += 100u;
        destination += 100u;
    }
    while (source != end);

    return reinterpret_cast<char*>(destination);
}

HOOK(0x0044C150, FixedRecords_ProcessRange, "auto");
