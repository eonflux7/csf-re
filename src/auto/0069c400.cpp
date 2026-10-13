#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __cdecl, _strstr, 0x007B2350, (char* Str, char* SubStr));

CSF_GLOBAL(uint8_t, g_ScenarioPathEntries, 0x008A66C8);
CSF_GLOBAL(uint8_t, g_ScenarioPathEntriesEnd, 0x008A677C);

static char* __cdecl CoreWin32Debug_ApplyScenarioPath(
    char* path, int /*a2*/, int scenarioId, int /*a4*/,
    int /*a5*/, int /*a6*/, int /*a7*/)
{
    uint8_t* entry = &g_ScenarioPathEntries;
    while (CSF_AT(int, entry, 0) != scenarioId)
    {
        entry += 12;
        if (reinterpret_cast<intptr_t>(entry) >=
            reinterpret_cast<intptr_t>(&g_ScenarioPathEntriesEnd))
        {
            return reinterpret_cast<char*>(entry);
        }
    }

    if (!entry)
        return reinterpret_cast<char*>(entry);

    char* insertion = CSF_AT(char*, entry, 4);
    if (!*insertion)
        return insertion;

    char* match = _strstr(
        path, const_cast<char*>(CSF_STR(0x0085E8B0, "\\SCENES")));
    if (!match)
        return match;

    const char* suffix = match + 7;
    alignas(8) uint8_t rebuiltPathBytes[0xFC];
    char* rebuiltPath = reinterpret_cast<char*>(rebuiltPathBytes);
    std::memset(rebuiltPath, 0, 248 + sizeof(int16_t));
    std::memcpy(rebuiltPath, path, static_cast<size_t>(suffix - path));

    insertion = CSF_AT(char*, entry, 4);
    size_t copyLength = std::strlen(insertion) + 1;
    char* appendPosition = rebuiltPath;
    while (*appendPosition)
        ++appendPosition;
    std::memcpy(appendPosition, insertion, copyLength);

    copyLength = std::strlen(suffix) + 1;
    appendPosition = rebuiltPath;
    while (*appendPosition)
        ++appendPosition;
    std::memcpy(appendPosition, suffix, copyLength);

    char* source = rebuiltPath;
    char* destination = path;
    char character;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character);

    // The original returns the address just past the temporary's terminator.
    return reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(source));
}

HOOK(0x0069C400, CoreWin32Debug_ApplyScenarioPath, "auto");
