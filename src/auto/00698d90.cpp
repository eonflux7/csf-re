#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __cdecl CoreSortedTable_ParseDescriptionId(const char* description)
{
    if (!description)
        return 0;

    const int8_t first = static_cast<int8_t>(description[0]);
    if (!first)
        return 0;

    const char* end = description;
    while (*end)
        ++end;

    if (end - description != 4)
        return 0;

    uint32_t id = static_cast<uint32_t>(static_cast<int32_t>(first));
    id = (id << 8) |
         static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(description[1])));
    id = (id << 8) |
         static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(description[2])));
    id = (id << 8) |
         static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(description[3])));

    return static_cast<int32_t>(id);
}

HOOK(0x00698D90, CoreSortedTable_ParseDescriptionId, "auto");
