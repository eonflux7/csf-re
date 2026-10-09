#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the address of the 48-byte record at position `index` inside the
// table whose base pointer is stored at offset 0xE8 of the object.
static int __fastcall GetRecordAddressByIndex(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0xE8);
    return tableBase + 48 * index;
}

HOOK(0x44FDB0, GetRecordAddressByIndex, "auto");
