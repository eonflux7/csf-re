#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AssignPointerVector_715E70, 0x00715E70,
         (void* self, void* edx, int source));

static int __cdecl Transfer16ByteElementsBackward(
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
        AssignPointerVector_715E70(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x007165F0, Transfer16ByteElementsBackward, "auto");
