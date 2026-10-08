#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AssignPointerVector_715E70, 0x00715E70,
         (void* self, void* edx, const void* source));

static int __cdecl Copy16ByteElementRange(int sourceBegin, int sourceEnd,
                                         int destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination =
        static_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        AssignPointerVector_715E70(
            reinterpret_cast<void*>(destination),
            nullptr,
            reinterpret_cast<const void*>(source));
        source += 16u;
        destination += 16u;
    }
    while (source != end);

    return static_cast<int>(destination);
}

HOOK(0x007165B0, Copy16ByteElementRange, "auto");
