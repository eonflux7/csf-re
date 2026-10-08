#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0,
         (char* source, char* destination, unsigned int capacity));
CSF_FUNC(bool, __cdecl, ResourceNamesMatch, 0x00694AA0,
         (char* first, char* second));

static int __fastcall C_008655e8_FindRasterIndex(
    C_008655e8* self, void* /*edx*/, char* source)
{
    if (!source)
        return -1;

    char normalizedName[256] = {};
    NormalizeResourceName(source, normalizedName, 256);

    const std::uint32_t begin = self->field_0048;
    const std::uint32_t count = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - begin) / 36)
        : 0;

    std::uint32_t entryOffset = 0;
    for (std::uint32_t index = 0; index < count; ++index, entryOffset += 36)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            self->field_0048 + entryOffset);

        char* storedName;
        if (CSF_AT(std::uint32_t, entry, 0x1c) < 16)
            storedName = reinterpret_cast<char*>(entry + 8);
        else
            storedName = CSF_AT(char*, entry, 0x08);

        // Deliberately compare the original input, not the normalized buffer.
        if (ResourceNamesMatch(source, storedName))
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x00727020, C_008655e8_FindRasterIndex, "auto");
