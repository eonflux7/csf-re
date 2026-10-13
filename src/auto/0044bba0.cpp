#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of entry `index` in the table whose base pointer is stored at
// this+0x10. Each entry is 0x64 (100) bytes.
static int __fastcall GetEntryAddressByIndex(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0x10);
    return tableBase + index * 0x64;
}

HOOK_KEEP_ECX(0x0044BBA0, GetEntryAddressByIndex, 1, "auto");
