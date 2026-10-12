#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Initializes the RpTeam base part of C_008630e0: runs the base constructor,
// installs this class's vtable, and zeroes the team state fields (field_0020 = 10).
static int __fastcall C_008630e0_InitBase(C_008630e0* self, void* /*edx*/) {
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    *reinterpret_cast<void**>(self) = CSF_VTBL(0x008630E0);
    self->field_0008 = 0;
    CSF_AT(uint8_t, self, 0x0C) = 0;   // LOBYTE(field_000c)
    CSF_AT(uint8_t, self, 0x0D) = 0;   // BYTE1(field_000c)
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0004 = 0;
    self->field_0010 = 0;
    self->field_0011 = 0;
    self->field_0012 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x006FF930, C_008630e0_InitBase, "auto");
