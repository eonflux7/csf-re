#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, GlobalTable_LookupRecord, 0x00769360, (void* self, void* edx, unsigned int a2));

// Finds the index of the first entry in the table whose leading word matches
// the leading word of the script token, skipping null entries, the token itself,
// and entries whose second word is -1. Returns -1 when nothing matches.
static int __fastcall S_0046f130_FindEntryIndex(S_0046f130* self, void* /*edx*/, __int16* a2)
{
    // The key is read before the global lookup and handed to the lookup as its stack argument.
    unsigned int lookupKey = static_cast<unsigned int>(static_cast<int>(*a2));
    int globalValue = GetGlobalValue_768E70();
    void* record = reinterpret_cast<void*>(GlobalTable_LookupRecord(reinterpret_cast<void*>(globalValue), nullptr, lookupKey));

    if (CSF_AT(int, record, 0x18) != 0)
        return -1;

    uint32_t entriesBegin = self->field_0004;
    unsigned int entryCount = 0;
    if (entriesBegin != 0)
        entryCount = static_cast<unsigned int>(static_cast<int>(self->field_0008 - entriesBegin) >> 2);

    int result = 0;
    if (entryCount == 0)
        return -1;

    __int16** entry = reinterpret_cast<__int16**>(entriesBegin);
    for (;; ++entry)
    {
        __int16* candidate = *entry;
        if (candidate)
        {
            if (candidate != a2 && candidate[1] != -1 && candidate[0] == *a2)
                break;
        }
        if (static_cast<unsigned int>(++result) >= entryCount)
            return -1;
    }
    return result;
}

HOOK(0x0046F130, S_0046f130_FindEntryIndex, "auto");
