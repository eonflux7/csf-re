#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of entry `index` in the table whose base pointer is stored at
// this+0x24. Each entry is 0x38 (56) bytes.
static int __fastcall Campaign_LookupBriefingEntry(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0x24);
    return tableBase + index * 0x38;
}

HOOK(0x0043E590, Campaign_LookupBriefingEntry, "auto");
