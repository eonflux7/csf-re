#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall S_00452980_FindEntryIndexByName(
    S_00452980* self, void* /*edx*/, char* name)
{
    const uint32_t begin = self->field_00cc;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_00d0 - begin) / 48)
        : 0;

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 48)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_00cc) + offset;
        const char* entryName = CSF_AT(uint32_t, entry, 0x2C) < 0x10
            ? reinterpret_cast<const char*>(entry + 0x18)
            : CSF_AT(const char*, entry, 0x18);

        if (name && entryName && __stricmp(name, entryName) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x00452980, S_00452980_FindEntryIndexByName, "auto");
