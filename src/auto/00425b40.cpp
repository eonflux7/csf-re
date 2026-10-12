#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of entry `index` in a 20-byte-stride record table whose
// base pointer is stored at +0x14 of this object. Class not yet identified.
static int __fastcall RecordTable_GetEntryAddress(void* self, void* /*edx*/, int index) {
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x14);
    return static_cast<int>(tableBase + static_cast<uint32_t>(index) * 20u);
}

HOOK(0x00425B40, RecordTable_GetEntryAddress, "auto");
