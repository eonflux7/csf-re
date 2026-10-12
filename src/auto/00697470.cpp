#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));
CSF_FUNC(int, __cdecl, _toupper, 0x007B1B21, (int C));
CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

static bool __cdecl CoreFiles_RemoveBaseDirectoryPrefix(char* path)
{
    if (!path)
        return false;

    const size_t pathLength = std::strlen(path);
    void* options = g_pOptions;
    const char* prefix = CSF_AT(uint32_t, options, 0x1C) < 0x10u
        ? reinterpret_cast<const char*>(options) + 0x08
        : CSF_AT(const char*, options, 0x08);
    const size_t prefixLength = std::strlen(prefix);

    if (pathLength < prefixLength)
        return false;

    size_t index = 0;
    while (index < prefixLength)
    {
        prefix = CSF_AT(uint32_t, options, 0x1C) < 0x10u
            ? reinterpret_cast<const char*>(options) + 0x08
            : CSF_AT(const char*, options, 0x08);

        const int prefixUpper = _toupper(static_cast<int8_t>(prefix[index]));
        const int pathUpper = _toupper(static_cast<int8_t>(path[index]));
        options = g_pOptions;

        if (pathUpper != prefixUpper)
        {
            const char pathCharacter = path[index];
            if (pathCharacter != 92 && pathCharacter != 47)
                return false;

            prefix = CSF_AT(uint32_t, options, 0x1C) < 0x10u
                ? reinterpret_cast<const char*>(options) + 0x08
                : CSF_AT(const char*, options, 0x08);

            if (pathCharacter == 92)
            {
                if (prefix[index] != 47)
                    return false;
            }
            else if (prefix[index] != 92)
            {
                return false;
            }
        }

        ++index;
    }

    alignas(8) uint8_t destinationBytes[0x104];
    char* destination = reinterpret_cast<char*>(destinationBytes);
    std::memset(destination, 0, 0x100);
    _strncpy(destination, path + index, std::strlen(path) - index);
    destination[std::strlen(path) - index] = 0;

    char* output = path;
    const char* source = destination;
    char character;
    do
    {
        character = *source++;
        *output++ = character;
    } while (character != 0);

    return true;
}

HOOK(0x00697470, CoreFiles_RemoveBaseDirectoryPrefix, "auto");
