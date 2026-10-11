#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x00711A30: called with ecx = this->field_0014 and one stack argument (a2).
CSF_FUNC(char, __fastcall, TransformEffectValue, 0x00711A30, (void* self, void* edx, char a2));

// Original 0x0070F470: __thiscall, one stack argument, ret 4.
static char __fastcall C_00863ba8_SetEffectValue(C_00863ba8* self, void* /*edx*/, char a2) {
    char result;
    if (self->field_0014) {
        // The callee receives the field_0014 value as its `this` (ecx) and a2 on the stack.
        result = TransformEffectValue(reinterpret_cast<void*>(self->field_0014), nullptr, a2);
    } else {
        result = a2;
    }
    // The raw argument (not the transformed result) is stored.
    self->field_0032 = static_cast<uint8_t>(a2);
    return result;
}

HOOK(0x0070F470, C_00863ba8_SetEffectValue, "auto");
