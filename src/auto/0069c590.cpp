#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));

CSF_GLOBAL(uint8_t, g_EmptyScenarioSuffix, 0x0082E65C);
CSF_GLOBAL(uint32_t, g_SoundPathPart4, 0x0085E8C4);
CSF_GLOBAL(uint32_t, g_SoundPathPart8, 0x0085E8C8);
CSF_GLOBAL(uint16_t, g_SoundPathPart12, 0x0085E8CC);
CSF_GLOBAL(int, g_CurrentScenarioId, 0x008BAF98);
CSF_GLOBAL(int, g_FallbackScenarioId, 0x008BAF9C);
CSF_GLOBAL(int, g_CachedSoundScenarioId, 0x008BAFA0);

static int __cdecl CoreWin32Debug_GetSoundScenarioId()
{
    int result = g_CachedSoundScenarioId;
    if (result != 0)
        return result;

    const int scenarioId = g_CurrentScenarioId;
    const char* suffix =
        reinterpret_cast<const char*>(&g_EmptyScenarioSuffix);

    auto* entry =
        reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008A66C8));
    const auto entriesEnd =
        static_cast<intptr_t>(CSF_ADDR_DATA(0x008A677C));

    for (;;)
    {
        if (CSF_AT(int, entry, 0) == scenarioId)
        {
            if (entry != nullptr)
            {
                const char* candidate = CSF_AT(char*, entry, 4);
                if (candidate[0] != '\0')
                    suffix = candidate;
            }
            break;
        }

        entry += 12;
        if (reinterpret_cast<intptr_t>(entry) >= entriesEnd)
            break;
    }

    alignas(8) uint8_t pathBytes[0x64];
    char* path = reinterpret_cast<char*>(pathBytes);

    std::memcpy(path, CSF_STR(0x0085E8C0, "SOUNDS\\SCENES"), 4);
    std::memcpy(path + 4, &g_SoundPathPart4, 4);
    std::memcpy(path + 8, &g_SoundPathPart8, 4);
    std::memcpy(path + 12, &g_SoundPathPart12, 2);

    const std::size_t suffixBytes = std::strlen(suffix) + 1;
    char* destination = path;
    while (*destination != '\0')
        ++destination;
    std::memcpy(destination, suffix, suffixBytes);

    const char exists = Campaign_CheckScenarioFile(path);
    result = g_CurrentScenarioId;
    if (exists == 0)
        result = g_FallbackScenarioId;

    g_CachedSoundScenarioId = result;
    return result;
}

HOOK(0x0069C590, CoreWin32Debug_GetSoundScenarioId, "auto");
