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

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Walks the 60-byte entries between field_0008 and field_000c and returns the index
// of the first entry whose name matches String2 case-insensitively, or -1 (0xFFFFFFFF).
static unsigned int __fastcall GfxClumpDict_FindEntryIndexByName(S_006d4450* self, void* /*edx*/, const char* String2)
{
    unsigned int index = 0;
    int offset = 0;
    for (;; offset += 60)
    {
        uint32_t first = self->field_0008;
        if (first == 0)
            break;

        int count = static_cast<int>(self->field_000c - first) / 60;
        if (index >= static_cast<unsigned int>(count))
            break;

        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008 + offset));
        uint32_t nameLength = CSF_AT(uint32_t, entry, 0x18);

        const char* name;
        if (nameLength < 0x10)
            name = reinterpret_cast<const char*>(entry + 4);
        else
            name = CSF_AT(const char*, entry, 4);

        if (name && String2 && __stricmp(name, String2) == 0)
            return index;

        ++index;
    }
    return 0xFFFFFFFFu;
}

HOOK(0x6D46A0, GfxClumpDict_FindEntryIndexByName, "auto");
