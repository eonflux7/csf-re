#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Getter: reads the object pointer stored at self+0x10; if non-null it returns the
// 32-bit field at +0x14 of that referenced object, otherwise returns 0.
static int __fastcall GetReferencedField14(void* self, void* /*edx*/)
{
    uint32_t referenced = CSF_AT(uint32_t, self, 0x10);
    if (referenced)
        return static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(referenced)), 0x14));
    return 0;
}

HOOK(0x6B8270, GetReferencedField14, "auto");
