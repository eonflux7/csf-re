#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// __stricmp (MSVC CRT), cdecl, pops nothing from the caller's view.
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Finds the index of the entry whose name matches String1 (case-insensitive).
// Each entry is 36 bytes: name stored inline at +4 when its capacity (+0x18) is < 16,
// otherwise a pointer to the name is stored at +4.
// Returns the entry index, or 0xFFFFFFFF if no entry matches.
static unsigned int __fastcall C_00860954_FindIndexByName(C_00860954* self, void* /*edx*/, char* String1)
{
    unsigned int index = 0;
    for (int offset = 0; ; offset += 36)
    {
        uint32_t tableStart = self->field_0008;
        if (!tableStart)
            break;
        int32_t entryCount = static_cast<int32_t>(self->field_000c - tableStart) / 36;
        if (index >= static_cast<uint32_t>(entryCount))
            break;

        tableStart = self->field_0008;
        uint32_t entry = tableStart + static_cast<uint32_t>(offset);
        uint32_t capacity = CSF_AT(uint32_t, entry, 0x18);

        const char* entryName = (capacity < 0x10)
            ? reinterpret_cast<const char*>(entry + 4)
            : CSF_AT(const char*, entry, 4);

        if (String1 && entryName && __stricmp(String1, entryName) == 0)
            return index;

        ++index;
    }
    return 0xFFFFFFFFu;
}

HOOK(0x6C8DD0, C_00860954_FindIndexByName, "auto");
