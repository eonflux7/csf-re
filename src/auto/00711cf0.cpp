#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets a C_00863d80 (SOUNDMAX / DirectSound effect) to its default state and
// returns the object's address (its vtable slot). __thiscall, 2 stack args -> ret 8.
static uint32_t* __fastcall C_00863d80_Init(C_00863d80* self, void* /*edx*/, int handleValue, char enabledFlag)
{
    self->field_0004 = static_cast<uint32_t>(handleValue);
    self->field_01b2 = static_cast<uint8_t>(enabledFlag);
    self->field_0010 = 1.0f;
    self->field_0014 = 1.0f;
    self->field_0018 = 0;
    self->field_01a8 = 0x3F800000u;   // bit pattern of 1.0f
    self->field_01b1 = 0;
    self->field_00e0 = 0;
    self->field_01ac = 0x3F800000u;   // bit pattern of 1.0f
    self->field_01b0 = 0;
    self->field_01b3 = 0;
    self->field_01b4 = 0;
    self->vtbl = CSF_VTBL(0x00863D80);
    self->field_01b5 = 1;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x711CF0, C_00863d80_Init, "auto");
