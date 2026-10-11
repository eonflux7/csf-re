#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer called first by the constructor (original ret 8, __thiscall).
CSF_FUNC(int, __fastcall, C_0083e330_InitBaseChain, 0x004E2F50, (void* self, void* edx, int a2, int a3));

// Constructor body for C_0083e330: runs the base init, clears the trailing fields,
// stores the vtables, and returns this.
static C_0083e330* __fastcall C_0083e330_Construct(C_0083e330* self, void* /*edx*/, int a2, int a3) {
    C_0083e330_InitBaseChain(self, nullptr, a2, a3);
    self->field_004c = 0;
    self->field_0050 = 0;
    self->vtbl = CSF_VTBL(0x0083E330);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083E314));
    return self;
}

HOOK(0x004E1640, C_0083e330_Construct, "auto");
