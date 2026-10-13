#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));
CSF_FUNC(int, __cdecl, AppConfig_GetLanguageName, 0x0069C3C0, (int a1));

CSF_GLOBAL(uint16_t, g_GlobalNameTail, 0x0085E8E0);
CSF_GLOBAL(char, g_GlobalNameTerminator, 0x0085E8E2);
CSF_GLOBAL(char, g_PakExtensionTerminator, 0x00830FF0);
CSF_GLOBAL(uint32_t, g_ConfiguredOptionEntries, 0x008A66C8);
CSF_GLOBAL(int, g_PreparedConfiguredOption, 0x008BAF9C);

static int __cdecl CoreWin32Debug_PrepareConfiguredOption(int option)
{
    alignas(8) uint8_t pathBytes[0x200];
    char* path = reinterpret_cast<char*>(pathBytes);

    std::memset(path + 1, 0, 0x1FCu);
    CSF_AT(uint16_t, path, 0x1FD) = 0;
    path[0x1FF] = 0;
    std::memcpy(path, CSF_STR(0x0085E8DC, "Global"), 4);
    std::memcpy(path + 4, &g_GlobalNameTail, 2);
    path[6] = g_GlobalNameTerminator;

    const char* optionName = nullptr;
    int index = 0;
    uint8_t* entry = reinterpret_cast<uint8_t*>(&g_ConfiguredOptionEntries);

    for (;;)
    {
        if (CSF_AT(int, entry, 0) == option)
        {
            uint8_t* matchedEntry =
                reinterpret_cast<uint8_t*>(&g_ConfiguredOptionEntries) + 12 * index;
            if (matchedEntry != nullptr)
                optionName = CSF_AT(const char*, matchedEntry, 4);
            break;
        }

        entry += 12;
        ++index;
        if (reinterpret_cast<intptr_t>(entry) >=
            static_cast<intptr_t>(CSF_ADDR_DATA(0x008A677C)))
            break;
    }

    const size_t optionNameSize = std::strlen(optionName) + 1;
    char* destination = path + std::strlen(path);
    std::memcpy(destination, optionName, optionNameSize);

    destination = path + std::strlen(path);
    std::memcpy(destination, CSF_STR(0x00830FEC, ".pak"), 4);
    destination[4] = g_PakExtensionTerminator;

    int result = Campaign_CheckScenarioFile(path);
    if (static_cast<uint8_t>(result) != 0)
    {
        g_PreparedConfiguredOption = option;
    }
    else
    {
        result = AppConfig_GetLanguageName(option);
        g_PreparedConfiguredOption = 0;
    }
    return result;
}

HOOK(0x0069C660, CoreWin32Debug_PrepareConfiguredOption, "auto");
