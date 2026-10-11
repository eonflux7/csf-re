#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original: 0x006A74A0, __thiscall with 2 stack args (ret 8).
// If the dword at +8 is zero, store the tag 0x494C3830 at +0x2B0.
// Always returns a byte 0.
static char __fastcall NetGathering_SetDdlTagIfUnset(void* self, void* /*edx*/, int /*a2*/, int /*a3*/) {
    if (CSF_AT(uint32_t, self, 0x8) == 0) {
        CSF_AT(uint32_t, self, 0x2B0) = 0x494C3830u;
    }
    return 0;
}

HOOK(0x6A74A0, NetGathering_SetDdlTagIfUnset, "auto");
