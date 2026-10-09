#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans the table of entry pointers in self->field_0018 (self->field_0020 entries).
// Each entry points to a record whose dword at +4 is a key. Returns the first
// matching entry (the stored pointer value), or 0 if none matches.
static int __fastcall SceneFallbackLookup(S_004686a0* self, void* /*edx*/, int a2)
{
    uint32_t entryCount = self->field_0020;
    if (entryCount == 0)
        return 0;

    uint32_t* entryTable = reinterpret_cast<uint32_t*>(self->field_0018);
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        uint32_t entry = entryTable[index];
        if (entry != 0 && CSF_AT(int, reinterpret_cast<void*>(entry), 4) == a2)
            return static_cast<int>(entryTable[index]);
    }
    return 0;
}

HOOK(0x004686A0, SceneFallbackLookup, "auto");
