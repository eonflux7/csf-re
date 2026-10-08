#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Per-element copy/handling routine: __thiscall with one stack argument
// (this in ecx, source pointer pushed) so it pops 4 stack bytes.
CSF_FUNC(int, __fastcall, AssignWideStringRecords, 0x00723950,
         (void* self, void* edx, const void* source));

// Copies a range [sourceBegin, sourceEnd) of 16-byte elements into
// consecutive elements starting at destinationBegin, applying the
// element routine to each one.
static int __cdecl Copy16ByteElementRange(int sourceBegin, int sourceEnd,
                                          int destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        AssignWideStringRecords(
            reinterpret_cast<void*>(destination),
            nullptr,
            reinterpret_cast<const void*>(source));
        source += 16u;
        destination += 16u;
    }
    while (source != end);

    return static_cast<int>(destination);
}

HOOK(0x00723C10, Copy16ByteElementRange, "auto");
