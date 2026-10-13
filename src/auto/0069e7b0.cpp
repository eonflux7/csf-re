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

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Looks up a[2] entry value in the owner's pointer list (+0x2c, count at +0x34).
// If found: releases the entry via its vtable slot 0 (arg 1), clears the slot,
// then removes it from the list by shifting the tail down.
static char __fastcall CoreFiles_RemoveEntryValue(void* self, void* /*edx*/, int a2)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x34);
    char result = 0;
    if (count == 0)
        return result;

    uint32_t* entries = CSF_AT(uint32_t*, self, 0x2c);
    uint32_t index = 0;
    uint32_t* scan = entries;
    while (static_cast<uint32_t>(a2) != *scan)
    {
        ++index;
        if (index >= count)
            return 0;
        ++scan;
    }

    void* entry = reinterpret_cast<void*>(entries[index]);
    if (entry)
    {
        // entry->vtbl[0](entry, 1)
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
        uint32_t* refreshedEntries = CSF_AT(uint32_t*, self, 0x2c);
        refreshedEntries[index] = 0;
    }

    uint32_t remaining = CSF_AT(uint32_t, self, 0x34) - 1;
    CSF_AT(uint32_t, self, 0x34) = remaining;
    if (index != remaining)
    {
        uint32_t* list = CSF_AT(uint32_t*, self, 0x2c);
        _memcpy(list + index,
                list + index + 1,
                static_cast<std::size_t>(remaining - index) * 4);
    }
    return 1;
}

HOOK(0x0069E7B0, CoreFiles_RemoveEntryValue, "auto");
