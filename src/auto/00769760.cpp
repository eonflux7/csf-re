#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Searches a table of 68-byte symbol records (begin at +0x08, end at +0x0C) for a
// case-insensitive match of the given name. Returns the record index, or -1.
static int __fastcall MissionScript_LookupSymbolId(void* self, void* /*edx*/, char* String1)
{
    uint32_t tableBegin = CSF_AT(uint32_t, self, 0x08);
    uint32_t symbolCount = 0;
    if (tableBegin != 0) {
        int32_t byteSpan = (int32_t)(CSF_AT(uint32_t, self, 0x0C) - tableBegin);
        symbolCount = (uint32_t)(byteSpan / 68);
    }

    if (symbolCount == 0) {
        return -1;
    }

    uint32_t index = 0;
    while (String1 == nullptr || __stricmp(String1, (const char*)&g_byte_82E65C) != 0) {
        if (++index >= symbolCount) {
            return -1;
        }
    }
    return (int)index;
}

HOOK(0x00769760, MissionScript_LookupSymbolId, "auto");
