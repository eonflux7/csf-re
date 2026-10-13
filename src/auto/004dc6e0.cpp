#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of record `index` in the table whose base pointer is stored
// in the first dword of this. Each record is 0x1C (28) bytes.
static int __fastcall EntryState_ResolveRecord(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0x00);
    return tableBase + index * 0x1C;
}

HOOK(0x004DC6E0, EntryState_ResolveRecord, "auto");
