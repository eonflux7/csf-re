#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CompareNamesIgnoreCase, 0x007B0C41,
         (const char* left, const char* right));

static int __fastcall C_00864508_FindValueByName(
    C_00864508* self, void* /*edx*/, char* name)
{
    const uint32_t begin = self->field_0014;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0018 - begin) >> 6)
        : 0;

    if (count == 0)
        return 0;

    uint32_t index = 0;
    uint32_t byteOffset = 0;

    for (;;)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            self->field_0014 + byteOffset);

        const char* entryName;
        if (CSF_AT(uint32_t, entry, 0x18) < 0x10u)
            entryName = reinterpret_cast<const char*>(entry + 4);
        else
            entryName = CSF_AT(const char*, entry, 0x04);

        if (CompareNamesIgnoreCase(entryName, name) == 0)
        {
            // The comparison may change the entry array; reload its base.
            auto* matchedEntry = reinterpret_cast<uint8_t*>(
                self->field_0014 + (index << 6));
            return CSF_AT(int32_t, matchedEntry, 0x38);
        }

        ++index;
        byteOffset += 0x40;
        if (index >= count)
            return 0;
    }
}

HOOK(0x0071A5E0, C_00864508_FindValueByName, "auto");
