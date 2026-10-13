#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a flag byte (src+0) and a 32-bit value (src+4) into self+0x50 and self+0x54.
// Returns the copied 32-bit value.
static int __fastcall CopyFlagAndValueFromRecord(void* self, void* /*edx*/, void* src) {
    CSF_AT(uint8_t, self, 0x50) = CSF_AT(uint8_t, src, 0);
    int value = CSF_AT(int, src, 4);
    CSF_AT(int, self, 0x54) = value;
    return value;
}

HOOK(0x0073BDE0, CopyFlagAndValueFromRecord, "auto");
