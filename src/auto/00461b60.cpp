#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 0x18-byte record from source into self, installs the C_0082d448
// vtable at +0x4, and returns self (the original's "return this").
static void* __fastcall CopyTransformRecord(void* self, void* /*edx*/, void* source) {
    uint32_t* dst = static_cast<uint32_t*>(self);
    const uint32_t* src = static_cast<const uint32_t*>(source);

    dst[0] = src[0];
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D448);
    dst[2] = src[2];
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    dst[6] = src[6];
    return self;
}

HOOK(0x00461B60, CopyTransformRecord, "auto");
