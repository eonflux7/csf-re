#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies three dwords from the source record (a2) into self, then zeroes
// the two trailing dwords at self+0x24 and self+0x1C. Returns 0.
// No owning class is identified for this method, so self is untyped.
static int __fastcall CopyStateFieldsAndClearFlags(void* self, void* /*edx*/, void* a2) {
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, a2, 0x20);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, a2, 0x18);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, a2, 0x1C);
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    return 0;
}

HOOK(0x73B550, CopyStateFieldsAndClearFlags, "auto");
