#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the index of the first non-null entry in the table at field_0018 whose
// virtual slot 4 (vtbl+0x10) returns wantedValue, or -1 if none matches.
static int __fastcall C_00860288_FindIndexOfMatchingEntry(C_00860288* self, void* /*edx*/, int wantedValue)
{
    int index = 0;
    if (self->field_001c == 0)
        return -1;

    while (true)
    {
        uint32_t entryValue = reinterpret_cast<uint32_t*>(self->field_0018)[index];
        if (entryValue != 0)
        {
            void* entry = reinterpret_cast<void*>(entryValue);
            using EntryGetterFn = int(__fastcall*)(void* self, void* edx);
            EntryGetterFn getter = reinterpret_cast<EntryGetterFn>(CSF_AT(void**, entry, 0)[4]);
            if (getter(entry, nullptr) == wantedValue)
                return index;
        }

        ++index;
        if (static_cast<uint32_t>(index) >= static_cast<uint32_t>(self->field_001c))
            return -1;
    }
}

HOOK(0x6B2B90, C_00860288_FindIndexOfMatchingEntry, "auto");
