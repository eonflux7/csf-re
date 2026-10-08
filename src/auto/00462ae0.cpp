#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Initializes a 0x14-byte object: two sentinel dwords set to -1, a vtable
// pointer, then two dwords copied from the source object at offsets +4 and +8.
// Returns the object (this).
static void* __fastcall ResetPairAndCopyEndpoints(void* self, void* /*edx*/, const void* source)
{
    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(void*, self, 0x08) = CSF_VTBL(0x82D448);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, source, 0x08);
    return self;
}

HOOK(0x462AE0, ResetPairAndCopyEndpoints, "auto");
