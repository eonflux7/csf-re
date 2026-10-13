#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, Records_ProcessEntry, 0x0044C0C0,
         (void* self, void* edx, int a2));

static int __cdecl Records_ProcessBackward(
    int sourceBegin, int sourceEnd, int destinationEnd, int unusedArg)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = static_cast<std::uint32_t>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 100u;
        destination -= 100u;
        Records_ProcessEntry(
            reinterpret_cast<void*>(destination),
            nullptr,
            static_cast<int>(source));
    }
    while (source != begin);

    return static_cast<int>(destination);
}

HOOK(0x0044C190, Records_ProcessBackward, "auto");
