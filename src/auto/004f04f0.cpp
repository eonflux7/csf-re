#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of pending object `index` in a 36-byte-stride table whose
// base pointer is stored at +0x04 of this object.
static int __fastcall C_0085e7b8_GetPendingObject(C_0085e7b8* self, void* /*edx*/, int index) {
    uint32_t tableBase = self->field_0004;
    return static_cast<int>(tableBase + static_cast<uint32_t>(index) * 36u);
}

HOOK(0x004F04F0, C_0085e7b8_GetPendingObject, "auto");
