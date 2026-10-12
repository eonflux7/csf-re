#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, AnimationDefs_CopyEntry, 0x00773200,
         (void* self, void* edx, int a2));

static char* __cdecl AnimationDefs_CopyEntryRange(int sourceBegin, int sourceEnd,
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
        AnimationDefs_CopyEntry(reinterpret_cast<void*>(destination),
                                nullptr,
                                static_cast<int>(source));
        source += 96u;
        destination += 96u;
    }
    while (source != end);

    return reinterpret_cast<char*>(static_cast<std::uintptr_t>(destination));
}

HOOK(0x00773280, AnimationDefs_CopyEntryRange, "auto");
