#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor for C_0085edfc: stores the vtable, two constructor arguments,
// and zeroes the trailing field. Returns the object's address (&vtbl).
static void** __fastcall C_0085edfc_Init(C_0085edfc* self, void* /*edx*/,
                                         uint32_t firstArg, uint32_t secondArg,
                                         uint32_t thirdArg) {
    self->field_0004 = firstArg;
    self->vtbl = CSF_VTBL(0x0085EDFC);
    self->field_0008 = secondArg;
    self->field_000c = thirdArg;
    self->field_0010 = 0;
    return reinterpret_cast<void**>(self);
}

HOOK(0x6A29A0, C_0085edfc_Init, "auto");
