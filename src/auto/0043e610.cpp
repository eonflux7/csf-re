#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));

static bool __fastcall Campaign_HasCurrentScene(void* self, void* /*edx*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x34);
    uint32_t count = 0;
    if (begin != 0)
    {
        const int32_t byteLength =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x38) - begin);
        count = static_cast<uint32_t>(byteLength / 196);
    }

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index)
    {
        auto* entry = CSF_AT(uint8_t*, self, 0x34) + offset;
        char* sceneName = CSF_AT(uint32_t, entry, 0x38) < 0x10
            ? reinterpret_cast<char*>(entry + 0x24)
            : CSF_AT(char*, entry, 0x24);

        auto* state = reinterpret_cast<uint8_t*>(GetGlobalValue_8B0568());
        auto* currentScene = state + 0x7C;
        char* currentSceneName = CSF_AT(uint32_t, state, 0x94) < 0x10
            ? reinterpret_cast<char*>(currentScene + 4)
            : CSF_AT(char*, currentScene, 4);

        if (SceneNamesMatch(currentSceneName, sceneName))
            return true;

        offset += 196;
    }

    return false;
}

HOOK(0x0043E610, Campaign_HasCurrentScene, "auto");
