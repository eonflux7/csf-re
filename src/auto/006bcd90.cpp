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

// Collects sector objects of one clump handle into g_SectorObjects, returns status.
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));

CSF_GLOBAL(uint32_t, g_SectorObjectCount, 0x008A7E58);

// Three groups of 0x10-byte records starting at +0x34 (count) / +0x2C (handle array pointer).
// For each group, every handle is passed to the collector and the total object count is accumulated.
static int __fastcall C_008605d8_SelectNamedObject(uint32_t* self, void* /*edx*/, int /*unusedArg*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int groupTotals[3];

    for (int group = 0; group < 3; ++group)
    {
        const std::size_t recordOffset = static_cast<std::size_t>(group) * 0x10;
        groupTotals[group] = 0;

        if (CSF_AT(uint32_t, base, 0x34 + recordOffset) != 0)
        {
            uint32_t total = 0;
            uint32_t index = 0;
            do
            {
                uint32_t* handles = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, base, 0x2C + recordOffset));
                int clumpHandle = static_cast<int>(handles[index]);
                SceneLoader_CollectSectorObjects(clumpHandle,
                                                 static_cast<int>(CSF_ADDR_DATA(0x008A7E50)));
                total += g_SectorObjectCount;
                ++index;
            }
            while (index < CSF_AT(uint32_t, base, 0x34 + recordOffset));

            groupTotals[group] = static_cast<int>(total);
        }
    }

    return 0;
}

HOOK(0x006BCD90, C_008605d8_SelectNamedObject, "auto");
