#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original: char __thiscall sub_7381D0(C_00867318* this, char a2); ret 4
static char __fastcall C_00867318_SetStateByteMarkDirty(C_00867318* self, void* /*edx*/, char newValue) {
    char result = newValue;
    if (self->field_0034 != static_cast<uint8_t>(newValue)) {
        self->field_0010 = 1;
        self->field_0034 = static_cast<uint8_t>(newValue);
    }
    return result;
}

HOOK(0x7381D0, C_00867318_SetStateByteMarkDirty, "auto");
