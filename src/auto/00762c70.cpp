#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, sub_762990, 0x00762990, (void* self, void* edx, int a2));

static std::uint32_t* __cdecl CharacterDefs_ProcessRecordsBackward(
    int sourceBegin, int sourceEnd, std::uint32_t* destinationEnd, int unusedArg)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceEnd);
    std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(destinationEnd);
    const std::uint32_t begin = static_cast<std::uint32_t>(sourceBegin);

    if (source == begin)
        return destinationEnd;

    do
    {
        source -= 0x22Cu;
        destination -= 0x22C;
        sub_762990(destination, nullptr, static_cast<int>(source));
    }
    while (source != begin);

    return reinterpret_cast<std::uint32_t*>(destination);
}

HOOK(0x00762C70, CharacterDefs_ProcessRecordsBackward, "auto");
