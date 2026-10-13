#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns nonzero when the object's 0x170 flags have 0x40 set, otherwise
// returns 1 when bit 0 of the byte at +0x94 is clear and 0 when it is set.
static int __fastcall QueryObjectLockedState_48E210(void* self, void* /*edx*/) {
    if ((CSF_AT(uint8_t, self, 0x170) & 0x40) != 0) {
        return 1;
    }
    return (CSF_AT(uint8_t, self, 0x94) & 1) == 0 ? 1 : 0;
}

HOOK(0x0048E210, QueryObjectLockedState_48E210, "auto");
