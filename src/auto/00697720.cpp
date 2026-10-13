#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(char, __cdecl, PrefixBaseDirectoryIfRelative, 0x006975F0, (char* path));
CSF_FUNC(int, __cdecl, ZlibLog_CopyPathBackslashes, 0x006A02C0, (char* dst, const char* src, int));
CSF_FUNC(bool, __cdecl, CoreLog_PathExists, 0x006A04E0, (const char* lpFileName));

static char __cdecl CoreFiles_CheckScenarioFile(char* path)
{
    char fileName[0x100];
    char relativePath[0x100];

    if (!path || !*path)
        return 0;

    if (CSF_AT(std::uint8_t, g_pOptions, 0x68))
    {
        char* destination = fileName;
        char* source = path;
        char character;
        do
        {
            character = *source++;
            *destination++ = character;
        } while (character);

        return CoreLog_PathExists(fileName);
    }

    char* destination = relativePath;
    char* source = path;
    char character;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character);

    PrefixBaseDirectoryIfRelative(relativePath);
    ZlibLog_CopyPathBackslashes(fileName, relativePath, 0);
    return CoreLog_PathExists(fileName);
}

HOOK(0x00697720, CoreFiles_CheckScenarioFile, "auto");
