#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor body of C_0082d5e0 (__thiscall, no stack args, ret 0).
// Installs the base-class and own vtable pointers, then resets the
// member block to its default values before returning `this`.
static uint32_t* __fastcall C_0082d5e0_Construct(C_0082d5e0* self, void* /*edx*/)
{
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D428);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D5E0);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0082D5C4);

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0xA;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0xA;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0xA;
    self->field_0008 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004086B0, C_0082d5e0_Construct, "auto");
