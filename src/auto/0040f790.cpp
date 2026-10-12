#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of scene entry `index` in a 24-byte-stride table whose
// base pointer is stored at +0x10 of this object.
static int __fastcall CMenuMpGameMode_GetSceneEntry(CMenuMpGameMode* self, void* /*edx*/, int index) {
    uint32_t tableBase = self->field_0010;
    return static_cast<int>(tableBase + static_cast<uint32_t>(index) * 24u);
}

HOOK(0x0040F790, CMenuMpGameMode_GetSceneEntry, "auto");
