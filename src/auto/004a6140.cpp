#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Maps the state word at +0x2B0 of the object referenced by this->field_0008
// to a result code: 3 -> 27, 9 -> 36, anything else -> -1.
static int __fastcall C_00834640_MapStateToResultCode(C_00834640* self, void* /*edx*/) {
    void* stateOwner = reinterpret_cast<void*>(self->field_0008);
    uint32_t state = CSF_AT(uint32_t, stateOwner, 0x2B0);

    int result = -1;
    if (state == 3) {
        return 27;
    }
    if (state == 9) {
        return 36;
    }
    return result;
}

HOOK(0x4A6140, C_00834640_MapStateToResultCode, "auto");
