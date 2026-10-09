#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, WeaponDefs_CopyRecord, 0x00784150,
         (void* self, void* edx, int a2));

static char* __cdecl WeaponDefs_CopyRecordRange(void* sourceBegin, void* sourceEnd,
                                                char* destinationBegin)
{
    std::uint32_t source = reinterpret_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t end = reinterpret_cast<std::uint32_t>(sourceEnd);
    std::uint32_t destination = reinterpret_cast<std::uint32_t>(destinationBegin);

    if (source == end)
        return destinationBegin;

    do
    {
        WeaponDefs_CopyRecord(reinterpret_cast<void*>(destination),
                              nullptr,
                              static_cast<int>(source));
        source += 0x228u;
        destination += 0x228u;
    }
    while (source != end);

    return reinterpret_cast<char*>(destination);
}

HOOK(0x00784210, WeaponDefs_CopyRecordRange, "auto");
