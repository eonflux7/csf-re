#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 16-byte element assign/copy: __thiscall(this = destination, source). Callee pops 4 bytes.
CSF_FUNC(int, __fastcall, AssignWideStringRecords, 0x00723950,
         (void* self, void* edx, int source));

// Copies 16-byte elements backward from [sourceBegin, sourceEnd) into the block
// ending at destinationEnd, so that overlapping regions are handled safely.
static int __cdecl Move16ByteElementsBackward(
    int sourceBegin, int sourceEnd, int destinationEnd)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 16u;
        destination -= 16u;
        AssignWideStringRecords(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x00723C50, Move16ByteElementsBackward, "auto");
