#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Slot +0x8 of C_008673b0's vtable (index 2). Not covered by the typed wrapper, so it is called through the vtable directly.
static int __fastcall C_008673b0_InvokeSlot8WithFlag(C_008673b0* self, void* /*edx*/) {
    self->field_0044 = 1;
    int result = ((int(__fastcall*)(void*, void*))self->vtbl[8 / 4])(self, nullptr);
    self->field_0044 = 0;
    return result;
}

HOOK(0x00738C20, C_008673b0_InvokeSlot8WithFlag, "auto");
