#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall S_0046c260_FindEntryIndex(
    S_0046c260* self, void* /*edx*/, char* name)
{
    const uint32_t begin = self->field_0018;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_001c - begin) / 28)
        : 0;

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 28)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(self->field_0018 + offset));

        const char* entryName = CSF_AT(uint32_t, entry, 0x18) < 0x10
            ? reinterpret_cast<const char*>(entry + 4)
            : CSF_AT(const char*, entry, 0x04);

        if (name && entryName && __stricmp(name, entryName) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x0046C260, S_0046c260_FindEntryIndex, "auto");
