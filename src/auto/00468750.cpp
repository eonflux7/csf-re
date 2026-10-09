#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original sub_45AA50: __fastcall, ecx = the entry being tested. Returns nonzero when the entry is filtered out.
CSF_FUNC(int, __fastcall, IsEntryFilteredOut, 0x0045AA50, (void* self, void* edx));

// Scans the entry table for the first non-null, unfiltered entry whose key (entry->+0x10->+4) equals key.
// Returns that entry, or 0 if none matches.
static int __fastcall Scene_LookupEntryByKey(S_00468750* self, void* /*edx*/, int key)
{
    uint32_t entryCount = self->field_0030;
    uint32_t index = 0;

    if (entryCount == 0)
        return 0;

    for (; index < entryCount; ++index)
    {
        uint32_t* entryTable = reinterpret_cast<uint32_t*>(self->field_0028);
        uint32_t entryValue = entryTable[index];
        if (entryValue == 0)
            continue;

        void* entry = reinterpret_cast<void*>(entryValue);
        if (IsEntryFilteredOut(entry, nullptr) != 0)
            continue;

        uint32_t keyHolderValue = CSF_AT(uint32_t, entry, 0x10);
        uint32_t entryKey = CSF_AT(uint32_t, reinterpret_cast<void*>(keyHolderValue), 4);
        if (entryKey == static_cast<uint32_t>(key))
            return static_cast<int>(entryValue);
    }

    return 0;
}

HOOK(0x00468750, Scene_LookupEntryByKey, "auto");
