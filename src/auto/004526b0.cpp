#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Copies the two-slot state (fields +4, +8, +0x10, +0x14) from src into dst and
// stamps dst with the 0x82D448 vtable at +0 and +0xC. Returns dst (null-safe).
static uint32_t* __stdcall C_0082d448_CopyConstruct(uint32_t* dst, uint32_t* src) {
    uint32_t* result = dst;
    if (dst) {
        CSF_AT(void*, dst, 0x0) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, dst, 0x4) = CSF_AT(uint32_t, src, 0x4);
        CSF_AT(uint32_t, dst, 0x8) = CSF_AT(uint32_t, src, 0x8);
        CSF_AT(void*, dst, 0xC) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, dst, 0x10) = CSF_AT(uint32_t, src, 0x10);
        CSF_AT(uint32_t, dst, 0x14) = CSF_AT(uint32_t, src, 0x14);
    }
    return result;
}

HOOK(0x4526B0, C_0082d448_CopyConstruct, "auto");
