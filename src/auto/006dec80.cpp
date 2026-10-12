#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GfxTextureLoader_ProcessEntry, 0x006DE9B0,
         (void* self, void* edx, int a5));

static int __cdecl GfxTextureLoader_ProcessEntriesBackward(
    int sourceBegin, int sourceEnd, int destinationEnd, int /*unusedArg*/)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 16;
        destination -= 16;
        GfxTextureLoader_ProcessEntry(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x006DEC80, GfxTextureLoader_ProcessEntriesBackward, "auto");
