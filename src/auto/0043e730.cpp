#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));

static std::uint32_t* __fastcall Campaign_SelectSceneByName(
    std::uint32_t* self, void* /*edx*/, char* sceneName)
{
    if (!sceneName)
    {
        CSF_AT(std::int32_t, self, 0x1C) = -1;
        return nullptr;
    }

    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x34);
    std::uint32_t sceneCount = 0;
    if (begin)
    {
        const std::uint32_t byteCount =
            CSF_AT(std::uint32_t, self, 0x38) - begin;
        sceneCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteCount) / 196);
    }

    std::uint32_t offset = 0;
    for (std::uint32_t index = 0; index < sceneCount; ++index, offset += 196)
    {
        auto* scene = CSF_AT(std::uint8_t*, self, 0x34) + offset;
        char* name = CSF_AT(std::uint32_t, scene, 0x1C) < 0x10
            ? reinterpret_cast<char*>(scene + 8)
            : CSF_AT(char*, scene, 8);

        const bool matches = SceneNamesMatch(sceneName, name);
        if (matches)
        {
            CSF_AT(std::uint32_t, self, 0x1C) = index;
            return reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(matches));
        }
    }

    offset = 0;
    for (std::uint32_t index = 0; index < sceneCount; ++index, offset += 196)
    {
        auto* scene = CSF_AT(std::uint8_t*, self, 0x34) + offset;
        char* alternateName = CSF_AT(std::uint32_t, scene, 0x38) < 0x10
            ? reinterpret_cast<char*>(scene + 0x24)
            : CSF_AT(char*, scene, 0x24);

        const bool matches = SceneNamesMatch(sceneName, alternateName);
        if (matches)
        {
            CSF_AT(std::uint32_t, self, 0x1C) = index;
            return reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(matches));
        }
    }

    CSF_AT(std::int32_t, self, 0x1C) = -1;
    return self;
}

HOOK(0x0043E730, Campaign_SelectSceneByName, "auto");
