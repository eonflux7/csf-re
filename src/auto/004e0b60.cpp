#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Returns the dword stored at +0x8 of the object referenced by the pointer
// held at +0xC in *self, or 0 when that pointer is null.
static int __fastcall GetLinkedObjectValue(void* self, void* /*edx*/)
{
    uint32_t linked = CSF_AT(uint32_t, self, 0xC);
    if (linked)
        return CSF_AT(int, reinterpret_cast<void*>(linked), 8);
    return 0;
}

HOOK(0x4E0B60, GetLinkedObjectValue, "auto");
