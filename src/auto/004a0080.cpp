#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008343a4_FindMatchingEntry(
    C_008343a4* self, void* /*edx*/, void* referenceObject)
{
    int result = 0;
    std::uint32_t index = 0;

    do {
        if (index >= self->field_0018)
            break;

        void* entry = reinterpret_cast<void**>(self->field_0010)[index];
        const std::uint32_t entryKey = CSF_AT(std::uint32_t, entry, 0x04);

        if (entryKey == CSF_AT(std::uint32_t, referenceObject, 0x04))
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(entry));

        ++index;
    } while (result == 0);

    return result;
}

// LTCG callers (0x4A07E0, 0x4A08F0, 0x4A0940) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x004A0080, C_008343a4_FindMatchingEntry, 1, "auto");
