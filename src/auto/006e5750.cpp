#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

using std::size_t;

CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));
CSF_FUNC(int, __cdecl, _strncmp, 0x007B00B0, (const char* name, const char* prefix, std::size_t length));

static char* __cdecl GfxObjectInstances_BuildAlternateName(
    char* buffer, char* source, int alternateIndex)
{
    std::uint32_t length = 0;
    while (source[length] != '\0')
        ++length;

    if (length > 7 &&
        _strncmp(source + length - 7, CSF_STR(0x00861C74, "_Alt"), 4u) == 0)
    {
        if (alternateIndex <= 0)
        {
            char* result = _strncpy(buffer, source, length - 7);
            buffer[length - 7] = '\0';
            return result;
        }

        char* sourceCursor = source;
        char* destinationCursor = buffer;
        char character;
        do
        {
            character = *sourceCursor++;
            *destinationCursor++ = character;
        } while (character != '\0');

        return reinterpret_cast<char*>(static_cast<std::intptr_t>(
            _sprintf(buffer + length - 3,
                     CSF_STR(0x00861C6C, "%.3d"), alternateIndex)));
    }

    if (alternateIndex > 0)
    {
        return reinterpret_cast<char*>(static_cast<std::intptr_t>(
            _sprintf(buffer, CSF_STR(0x00861050, "%s_Alt%.3d"),
                     source, alternateIndex)));
    }

    char* sourceCursor = source;
    char* destinationCursor = buffer;
    char character;
    do
    {
        character = *sourceCursor++;
        *destinationCursor++ = character;
    } while (character != '\0');

    return sourceCursor;
}

HOOK(0x006E5750, GfxObjectInstances_BuildAlternateName, "auto");
