#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __cdecl, _strrchr, 0x007B2E90, (char* Str, int Ch));
CSF_GLOBAL(uint16_t, g_ExtensionSeparator, 0x0085E678);

static char __cdecl CorePaths_ReplaceExtension(char* path, int unused, const char* extension)
{
    (void)unused;

    if (*extension == 46)
        ++extension;

    char* dot = _strrchr(path, 46);
    if (dot)
    {
        if (*extension)
        {
            char* destination = dot + 1;
            char character;
            do
            {
                character = *extension;
                *destination = character;
                ++extension;
                ++destination;
            } while (character);
            return character;
        }

        *dot = 0;
        return static_cast<char>(reinterpret_cast<uintptr_t>(dot));
    }

    if (*extension)
    {
        char* end = path;
        while (*end)
            ++end;

        CSF_AT(uint16_t, end, 0) = g_ExtensionSeparator;

        const char* extensionEnd = extension;
        while (*extensionEnd)
            ++extensionEnd;
        const size_t copyCount = static_cast<size_t>(extensionEnd - extension) + 1;

        end = path;
        while (*end)
            ++end;

        std::memcpy(end, extension, copyCount);
        return static_cast<char>(copyCount);
    }

    return 0;
}

HOOK(0x006947D0, CorePaths_ReplaceExtension, "auto");
