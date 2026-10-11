// Reimplementation of 0x0075CE80: searches the waypoint spawn table (72-byte
// entries, name at +0x08, name length at +0x1C) for a case-insensitive name match.
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static uint32_t __fastcall WaypointGraph_FindSpawnByName(void* self, void* /*edx*/, char* searchName)
{
    uint32_t matchIndex = 0;

    for (uint32_t elementOffset = 0;; elementOffset += 72)
    {
        uint32_t begin = CSF_AT(uint32_t, self, 0x08);
        if (begin == 0)
            break;

        int32_t spanBytes = (int32_t)(CSF_AT(uint32_t, self, 0x0C) - begin);
        if (matchIndex >= (uint32_t)(spanBytes / 72))
            break;

        uint8_t* element = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08) + elementOffset);
        uint32_t nameLength = CSF_AT(uint32_t, element, 0x1C);

        const char* entryName;
        if (nameLength < 0x10)
            entryName = reinterpret_cast<const char*>(element + 8);
        else
            entryName = reinterpret_cast<const char*>(CSF_AT(uint32_t, element, 0x08));

        if (__stricmp(searchName, entryName) == 0)
            return matchIndex;

        ++matchIndex;
    }

    return 0xFFFFFFFFu;
}

HOOK(0x0075CE80, WaypointGraph_FindSpawnByName, "auto");
