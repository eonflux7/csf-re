#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Constructor of class C_0082d448 (3 stack args, ret 12). Copies the
// initial int, two words from a3 and three words from a4 into the object.
static void* __fastcall C_0082d448_Construct(void* self, void* /*edx*/,
                                             int initialValue,
                                             uint8_t* sourceRecord,
                                             uint8_t* sourceVector) {
    CSF_AT(int, self, 0x0) = initialValue;
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, self, 0x8) = CSF_AT(uint32_t, sourceRecord, 0x4);
    CSF_AT(uint32_t, self, 0xC) = CSF_AT(uint32_t, sourceRecord, 0x8);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, sourceVector, 0x0);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, sourceVector, 0x4);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, sourceVector, 0x8);
    return self;
}

HOOK(0x004203B0, C_0082d448_Construct, "auto");
