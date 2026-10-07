#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00864508_FindFirstMatchingEntry(
    C_00864508* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0048;
    const int32_t count = begin
        ? (static_cast<int32_t>(self->field_004c - begin) >> 2)
        : 0;

    int32_t index = count - 1;
    int matchingEntry = 0;
    if (index < 0)
        return 0;

    do
    {
        void* entry = reinterpret_cast<void**>(self->field_0048)[index];
        if (entry)
        {
            const uint8_t matches =
                (reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x28 / 4]))(entry, nullptr);

            if (matches)
            {
                matchingEntry = static_cast<int>(
                    reinterpret_cast<uint32_t*>(self->field_0048)[index]);
            }
        }
        --index;
    } while (index >= 0);

    return matchingEntry;
}

HOOK(0x0071AE70, C_00864508_FindFirstMatchingEntry, "auto");
