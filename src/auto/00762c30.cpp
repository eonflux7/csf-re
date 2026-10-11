#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __fastcall, CharacterDefs_CopyRecord, 0x00762990,
         (void* self, void* edx, int a2));

static char* __cdecl CharacterDefs_CopyRecordRange(int sourceBegin, int sourceEnd,
                                                   char* destinationBegin)
{
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = static_cast<std::uint32_t>(sourceEnd);
    char* destination = destinationBegin;

    if (source == end)
        return destinationBegin;

    do
    {
        CharacterDefs_CopyRecord(destination, nullptr, static_cast<int>(source));
        source += 0x22Cu;
        destination += 0x22C;
    }
    while (source != end);

    return destination;
}

HOOK(0x00762C30, CharacterDefs_CopyRecordRange, "auto");
