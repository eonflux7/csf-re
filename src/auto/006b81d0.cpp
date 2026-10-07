#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the dword at offset +4 of the object pointed to by the pointer stored at
// this+0x10, or 0 when that pointer is null.
static int __fastcall GetInnerValueAtOffset4(void* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (inner)
        return static_cast<int>(CSF_AT(uint32_t, inner, 0x4));
    return 0;
}

HOOK(0x6B81D0, GetInnerValueAtOffset4, "auto");
