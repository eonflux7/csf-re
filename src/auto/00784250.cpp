#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, WeaponDefs_CopyEntry, 0x00784150,
         (void* self, void* edx, int a2));

static void* __cdecl WeaponDefs_CopyEntriesBackward(
    void* sourceBegin, void* sourceEnd, void* destinationEnd, int unusedArg)
{
    const std::uint32_t begin = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(sourceBegin));
    std::uint32_t source = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(sourceEnd));
    std::uint32_t destination = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(destinationEnd));

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 0x228u;
        destination -= 0x228u;
        WeaponDefs_CopyEntry(
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(destination)),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(destination));
}

HOOK(0x00784250, WeaponDefs_CopyEntriesBackward, "auto");
