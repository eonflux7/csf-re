#include <cstdint>
#include "hook.h"

// Returns the dword stored at index `index` of the table pointed to by *self.
// The table pointer itself is the first dword of the object.
static int __fastcall GetIndexedTableValue(void* self, void* /*edx*/, int index)
{
    uint32_t table = *reinterpret_cast<uint32_t*>(self);
    return *reinterpret_cast<int*>(table + 4 * index);
}

HOOK(0x006AF8A0, GetIndexedTableValue, "auto");
