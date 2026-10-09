#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies six dwords (24 bytes) from src into dst when dst is non-null.
// Returns dst unchanged (null stays null).
static uint32_t* __stdcall InputBindings_CopyMenuElementIntoSlot(uint32_t* dst, uint32_t* src) {
    uint32_t* result = dst;
    if (dst) {
        CSF_AT(uint32_t, dst, 0x00) = CSF_AT(uint32_t, src, 0x00);
        CSF_AT(uint32_t, dst, 0x04) = CSF_AT(uint32_t, src, 0x04);
        CSF_AT(uint32_t, dst, 0x08) = CSF_AT(uint32_t, src, 0x08);
        CSF_AT(uint32_t, dst, 0x0C) = CSF_AT(uint32_t, src, 0x0C);
        CSF_AT(uint32_t, dst, 0x10) = CSF_AT(uint32_t, src, 0x10);
        CSF_AT(uint32_t, dst, 0x14) = CSF_AT(uint32_t, src, 0x14);
    }
    return result;
}

HOOK(0x7685C0, InputBindings_CopyMenuElementIntoSlot, "auto");
