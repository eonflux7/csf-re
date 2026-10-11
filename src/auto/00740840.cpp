#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ClearDwordField4, 0x0073F760, (void* self, void* edx));

// Resets the vtable to C_00867890 and, if the linked object at +0x8 is set,
// tail-calls ClearDwordField4 with that object as its `this` (ecx holds the field value).
static void __fastcall C_00867890_ResetVtableAndClearField4(C_00867890* self, void* /*edx*/) {
    *(void***)self = CSF_VTBL(0x00867890);
    uint32_t linkedObject = CSF_AT(uint32_t, self, 0x8);
    if (linkedObject != 0) {
        ClearDwordField4(reinterpret_cast<void*>(linkedObject), nullptr);
    }
}

HOOK(0x740840, C_00867890_ResetVtableAndClearField4, "auto");
