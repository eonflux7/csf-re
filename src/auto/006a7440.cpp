#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets a 4-byte state tag at +0x2B0 when the dword at +0x08 is zero.
// Always returns 0. Callee pops one stack argument (ret 4); a2 is unused.
static int __fastcall NetGathering_InitTagIfUnset(void* self, void* /*edx*/, int /*unusedArg*/) {
    if (CSF_AT(uint32_t, self, 0x08) == 0) {
        CSF_AT(uint32_t, self, 0x2B0) = 0x494C3830;
    }
    return 0;
}

HOOK(0x6A7440, NetGathering_InitTagIfUnset, "auto");
