#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the DWORD stored at offset +0x10 of the object pointed to by the
// pointer field at self+8. If that pointer is null, returns 0.
static int __fastcall GetLinkedObjectField10(void* self, void* /*edx*/)
{
    uint8_t* linkedObject = CSF_AT(uint8_t*, self, 0x8);
    if (linkedObject)
        return CSF_AT(int, linkedObject, 0x10);
    return 0;
}

HOOK(0x4DB5D0, GetLinkedObjectField10, "auto");
