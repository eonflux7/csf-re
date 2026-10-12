#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static unsigned int __cdecl CoreFiles_FindNameIndex(char* name, void* entries)
{
    const uint32_t begin = CSF_AT(uint32_t, entries, 0x4);
    uint32_t count = 0;
    if (begin)
    {
        const uint32_t difference = CSF_AT(uint32_t, entries, 0x8) - begin;
        count = static_cast<uint32_t>(static_cast<int32_t>(difference) >> 5);
    }

    uint32_t index = 0;
    uint32_t offset = 0;
    if (!count)
        return UINT32_MAX;

    for (;;)
    {
        const uint32_t currentBegin = CSF_AT(uint32_t, entries, 0x4);
        void* entry = reinterpret_cast<void*>(currentBegin + offset);
        const uint32_t capacity = CSF_AT(uint32_t, entry, 0x18);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(entry) + 0x4
            : CSF_AT(const char*, entry, 0x4);

        if (__stricmp(name, entryName) == 0)
            return index < count ? index : UINT32_MAX;

        ++index;
        offset += 32;
        if (index >= count)
            return UINT32_MAX;
    }
}

HOOK(0x006977C0, CoreFiles_FindNameIndex, "auto");
