#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, _isalpha, 0x007B1927, (int character));
CSF_GLOBAL(void*, g_BaseDirectoryString, 0x008BAF8C);

static char __cdecl PrefixBaseDirectoryIfRelative(char* path)
{
    if (std::strlen(path) < 2u)
        return 0;

    const std::int8_t firstCharacter =
        static_cast<std::int8_t>(path[0]);

    if (firstCharacter == 0x5C || firstCharacter == 0x2F)
        return 0;

    if (_isalpha(static_cast<int>(firstCharacter)) && path[1] == 0x3A)
        return 0;

    auto* directoryString =
        static_cast<unsigned char*>(g_BaseDirectoryString);

    const char* directory;
    if (CSF_AT(std::uint32_t, directoryString, 0x1C) < 0x10u)
        directory = reinterpret_cast<const char*>(directoryString + 8);
    else
        directory = CSF_AT(const char*, directoryString, 8);

    char combinedPath[256];
    char* destination = combinedPath;
    char character;
    do
    {
        character = *directory++;
        *destination++ = character;
    } while (character != 0);

    std::size_t remainingBytes = std::strlen(path) + 1u;
    destination = combinedPath;
    while (*destination != 0)
        ++destination;

    const char* source = path;
    while (remainingBytes >= 4u)
    {
        std::uint32_t word;
        std::memcpy(&word, source, sizeof(word));
        std::memcpy(destination, &word, sizeof(word));
        source += 4;
        destination += 4;
        remainingBytes -= 4u;
    }
    while (remainingBytes != 0)
    {
        *destination++ = *source++;
        --remainingBytes;
    }

    source = combinedPath;
    destination = path;
    do
    {
        character = *source++;
        *destination++ = character;
    } while (character != 0);

    return 1;
}

HOOK(0x006975F0, PrefixBaseDirectoryIfRelative, "auto");
