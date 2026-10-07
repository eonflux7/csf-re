#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the last element of a packed array whose element
// stride is 12 bytes: base(+0x4) + 12 * count(+0xC) - 12.
static int __fastcall GetLastElementAddress(void* self, void* /*edx*/)
{
    uint32_t count = CSF_AT(uint32_t, self, 0xC);
    uint32_t base  = CSF_AT(uint32_t, self, 0x4);
    return static_cast<int>(base + 12 * count - 12);
}

HOOK(0x41E7D0, GetLastElementAddress, "auto");
