#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the dword stored at the address held by the pointer at offset 0x10,
// or 0 when that pointer is null.
static int __fastcall C_6B81C0_GetFirstItem(void* self, void* /*edx*/)
{
    uint32_t* item = CSF_AT(uint32_t*, self, 0x10);
    if (item)
        return static_cast<int>(*item);
    return 0;
}

HOOK(0x6B81C0, C_6B81C0_GetFirstItem, "auto");
