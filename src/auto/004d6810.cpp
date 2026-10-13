#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Latches a target value and notifies the linked state object, but only the first time.
// The field at +0x44 starts at -1 (unset). If it is still -1, store a2 at +0x44 and the
// flag byte a3 at +0x55, then call vtable slot 0x6C (vPropagateState) on the object at +0x14
// with argument 0. If the field is already set, do nothing and return 0.
static int __fastcall C_0083c478_LatchTargetAndNotify(C_0083c478* self, void* /*edx*/, int a2, char a3) {
    int result = 0;

    if (self->field_0044 == static_cast<uint32_t>(-1)) {
        self->field_0044 = static_cast<uint32_t>(a2);
        self->field_0055 = static_cast<uint8_t>(a3);
        void* stateObject = reinterpret_cast<void*>(self->field_0014);
        // vPropagateState: vtable slot 0x6C / 4 = 27, __thiscall with one stack argument (0).
        result = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, stateObject, 0)[27])(stateObject, nullptr, 0);
    }

    return result;
}

HOOK(0x4D6810, C_0083c478_LatchTargetAndNotify, "auto");
