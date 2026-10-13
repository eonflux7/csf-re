#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Campaign_CheckScenarioFile, 0x00697720, (char* a1));
CSF_FUNC(int, __cdecl, ParserBlocks_FindValueByKey, 0x0069C380, (int a1, int a2));
CSF_FUNC(int, __cdecl, AppConfig_GetLanguageName, 0x0069C3C0, (int a1));
CSF_FUNC(int, __cdecl, CoreWin32Debug_SelectGlobalPackage, 0x0069C510, (char* a1));

CSF_GLOBAL(int, g_Value_8BAF98, 0x008BAF98);
CSF_GLOBAL(int, g_FallbackOption, 0x008BAF9C);
CSF_GLOBAL(uint16_t, g_GlobalPrefixTail, 0x0085E8E0);
CSF_GLOBAL(uint8_t, g_GlobalPrefixTerminator, 0x0085E8E2);
CSF_GLOBAL(uint8_t, g_PackageSuffixTerminator, 0x00830FF0);

static int __cdecl CoreWin32Debug_ApplyOption(int option)
{
    g_Value_8BAF98 = option;

    alignas(8) uint8_t pathBytes[0x200];
    char* packagePath = reinterpret_cast<char*>(pathBytes);

    std::memset(packagePath + 1, 0, 0x1FC);
    CSF_AT(uint16_t, packagePath, 0x1FD) = 0;
    packagePath[0x1FF] = 0;

    std::memcpy(packagePath, CSF_STR(0x0085E8DC, "Global"), 4);
    CSF_AT(uint16_t, packagePath, 4) = g_GlobalPrefixTail;
    packagePath[6] = static_cast<char>(g_GlobalPrefixTerminator);

    const uint32_t tableEnd = CSF_ADDR_DATA(0x008A677C);
    uint32_t cursor = CSF_ADDR_DATA(0x008A66C8);
    const char* optionName = nullptr;

    // The preceding, unused table scan has no side effects.
    for (;;)
    {
        if (CSF_AT(uint32_t, cursor, 0) == static_cast<uint32_t>(option))
        {
            if (cursor != 0)
                optionName = reinterpret_cast<const char*>(
                    CSF_AT(uint32_t, cursor, 4));
            break;
        }

        cursor += 12;
        if (static_cast<int32_t>(cursor) >= static_cast<int32_t>(tableEnd))
            break;
    }

    std::size_t nameBytes = std::strlen(optionName) + 1;
    char* appendPosition = packagePath + std::strlen(packagePath);
    std::memcpy(appendPosition, optionName, nameBytes);

    appendPosition = packagePath + std::strlen(packagePath);
    std::memcpy(appendPosition, CSF_STR(0x00830FEC, ".pak"), 4);
    appendPosition[4] = static_cast<char>(g_PackageSuffixTerminator);

    if (!Campaign_CheckScenarioFile(packagePath))
    {
        const int fallbackOption = g_FallbackOption;
        AppConfig_GetLanguageName(fallbackOption);

        g_Value_8BAF98 = fallbackOption;
        std::memcpy(packagePath, CSF_STR(0x0085E8DC, "Global"), 4);
        CSF_AT(uint16_t, packagePath, 4) = g_GlobalPrefixTail;
        packagePath[6] = static_cast<char>(g_GlobalPrefixTerminator);

        // The earlier cdecl argument remains on the original stack.
        optionName = reinterpret_cast<const char*>(
            ParserBlocks_FindValueByKey(fallbackOption, fallbackOption));

        nameBytes = std::strlen(optionName) + 1;
        appendPosition = packagePath + std::strlen(packagePath);
        std::memcpy(appendPosition, optionName, nameBytes);

        appendPosition = packagePath + std::strlen(packagePath);
        std::memcpy(appendPosition, CSF_STR(0x00830FEC, ".pak"), 4);
        appendPosition[4] = static_cast<char>(g_PackageSuffixTerminator);
    }

    return CoreWin32Debug_SelectGlobalPackage(packagePath);
}

HOOK(0x0069C760, CoreWin32Debug_ApplyOption, "auto");
