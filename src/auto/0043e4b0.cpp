#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));

static int __fastcall Campaign_FindMission(void* self, void* /*edx*/, char* sceneName)
{
    if (!sceneName)
        return 0;

    const uint32_t begin = CSF_AT(uint32_t, self, 0x34);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x38) - begin) / 196)
        : 0;

    if (!count)
        return 0;

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 196)
    {
        auto* mission = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x34) + offset);
        char* name = CSF_AT(uint32_t, mission, 0x1C) < 0x10
            ? reinterpret_cast<char*>(mission + 8)
            : CSF_AT(char*, mission, 8);

        if (SceneNamesMatch(sceneName, name))
            return static_cast<int>(CSF_AT(uint32_t, self, 0x34) + 196 * index);
    }

    offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 196)
    {
        auto* mission = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x34) + offset);
        char* alternateName = CSF_AT(uint32_t, mission, 0x38) < 0x10
            ? reinterpret_cast<char*>(mission + 36)
            : CSF_AT(char*, mission, 36);

        if (SceneNamesMatch(sceneName, alternateName))
            return static_cast<int>(CSF_AT(uint32_t, self, 0x34) + 196 * index);
    }

    return 0;
}

HOOK(0x0043E4B0, Campaign_FindMission, "auto");
