#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets the session-state fields of C_0085fe28 when its state is 4 and the
// attached sub-object is not in state 1. The attached object's virtual slot 0
// is invoked with argument 1 (its release/shutdown path) before the pointer is cleared.
static int __fastcall C_0085fe28_ResetIfStateFour(C_0085fe28* self, void* /*edx*/, int /*a2*/) {
    int result = 0;

    if (self->field_0008 == 4) {
        uint8_t* attachedObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0090));

        if (CSF_AT(int, attachedObj, 0x0C) != 1) {
            if (attachedObj) {
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, attachedObj, 0)[0])(attachedObj, nullptr, 1);
                self->field_0090 = 0;
            }
            self->field_0008 = 0;
            self->field_000c = 0;
            self->field_0038 = 0;
        }
    }

    return result;
}

HOOK(0x006A74E0, C_0085fe28_ResetIfStateFour, "auto");
