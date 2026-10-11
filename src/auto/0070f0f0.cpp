#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (names chosen here; signatures from the packet).
CSF_FUNC(int, __cdecl, AudioEffect_Create, 0x007DD4C0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, AudioEffect_Register, 0x007F0EE0, (int a1, int a2));

// Creates a SOUNDMAX audio effect object, sets its default parameters
// (0.01f, 0.0f, 1.0f) with the 0x40/0x20/0x10 flag bits, then registers it
// with the owner's sound context. Returns nonzero if the effect was created.
static int __fastcall C_00863ba8_CreateAndRegisterEffect(C_00863ba8* self, void* /*edx*/) {
    self->field_0008 = static_cast<uint32_t>(AudioEffect_Create(4, 0, 0));
    if (self->field_0008) {
        CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0008), 0x6C) = 0.01f;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x70) |= 0x40u;
        CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0008), 0x68) = 0.0f;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x70) |= 0x20u;
        CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0008), 0x64) = 1.0f;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x70) |= 0x10u;
        AudioEffect_Register(static_cast<int>(self->field_0004),
                             static_cast<int>(self->field_0008));
    }
    return self->field_0008 != 0;
}

HOOK(0x0070F0F0, C_00863ba8_CreateAndRegisterEffect, "auto");
