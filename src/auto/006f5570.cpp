#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards (a2, &outValue, a3) to the object's vtable slot 0x1C (index 7).
// outValue starts at -1 and is passed by address; the slot's result is returned.
static int __fastcall C_008627d8_CallSlot1CWithOutValue(C_008627d8* self, void* /*edx*/, int a2, int a3) {
    int outValue = -1;
    return ((int(__fastcall*)(void*, void*, int, int*, int))self->vtbl[0x1C / 4])(self, nullptr, a2, &outValue, a3);
}

HOOK(0x6F5570, C_008627d8_CallSlot1CWithOutValue, "auto");
