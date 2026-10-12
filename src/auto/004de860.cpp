#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of entry `index` in a 12-byte-stride record table whose
// base pointer is stored at +0x30 of this object. Class not yet identified.
static int __fastcall RecordTable_GetEntryAddress12(void* self, void* /*edx*/, int index) {
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x30);
    return static_cast<int>(tableBase + static_cast<uint32_t>(index) * 12u);
}

HOOK(0x004DE860, RecordTable_GetEntryAddress12, "auto");
