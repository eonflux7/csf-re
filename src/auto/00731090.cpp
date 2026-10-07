#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall: ecx = destination element address, one stack argument (source element address).
CSF_FUNC(int, __fastcall, Transfer16ByteElement, 0x00730EA0,
         (void* self, void* edx, int source));

// Moves 16-byte elements backwards (from the end of [destinationBegin, ...) towards
// the beginning), sourced from the range ending at sourceEnd.
static int __cdecl Transfer16ByteElementsBackward(
    int destinationBegin, int sourceEnd, int destinationEnd)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(destinationBegin);

    if (source == begin)
        return destinationEnd;

    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    do
    {
        source -= 16u;
        destination -= 16u;
        Transfer16ByteElement(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x00731090, Transfer16ByteElementsBackward, "auto");
