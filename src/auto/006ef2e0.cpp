#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns 1 when the flags dword at +0x30 of the object's inner pointer (at +4) has bit 0x4 set, else 0.
static int __fastcall GfxPTankCreate_IsFlag4Set(void* self, void* /*edx*/) {
    uint32_t inner = CSF_AT(uint32_t, self, 0x4);
    uint32_t flags = CSF_AT(uint32_t, reinterpret_cast<void*>(inner), 0x30);
    return (flags & 4) == 4;
}

HOOK(0x006EF2E0, GfxPTankCreate_IsFlag4Set, "auto");
