#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a range of 36-byte string entries [sourceBegin, sourceEnd) into a destination
// array. Each entry's string is reassigned from the source, then two trailing fields
// (dword at +0x1C, word at +0x20) are copied. Returns the one-past-end destination pointer,
// or destinationBegin when the range is empty.
static int __cdecl CopyStringEntryRange(int sourceBegin, int sourceEnd, int destinationBegin)
{
    if (sourceBegin == sourceEnd)
        return destinationBegin;

    std::uint8_t* source = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(sourceBegin));
    std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(destinationBegin));
    const std::uintptr_t sourceLast = static_cast<std::uintptr_t>(sourceEnd);

    do
    {
        AssignStringSubstring(destination, nullptr, source, 0u, 0xFFFFFFFFu);
        CSF_AT(std::uint32_t, destination, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
        CSF_AT(std::uint16_t, destination, 0x20) = CSF_AT(std::uint16_t, source, 0x20);
        source += 0x24;
        destination += 0x24;
    } while (reinterpret_cast<std::uintptr_t>(source) != sourceLast);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(destination));
}

HOOK(0x006C8FE0, CopyStringEntryRange, "auto");
