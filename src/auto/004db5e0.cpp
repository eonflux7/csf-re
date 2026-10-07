#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the DWORD at offset +0x10 of the object pointed to by this+0x0C,
// or 0 when that pointer is null.
static int __fastcall GetLinkedObjectValue16(void* self, void* /*edx*/)
{
    uint32_t linked = CSF_AT(uint32_t, self, 0x0C);
    if (linked)
        return static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(linked), 0x10));
    return 0;
}

HOOK(0x4DB5E0, GetLinkedObjectValue16, "auto");
