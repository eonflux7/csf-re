#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer called first by the constructor of C_00840d64 (0x00519750, __thiscall, ret 0).
// Its result is unused by the original.
CSF_FUNC(uint32_t*, __fastcall, C_00840d54_InitBase, 0x00519750, (void* self, void* /*edx*/));

// Constructor for C_00840d64: runs the base initializer, installs this class's vtable,
// zeroes the fields at +0x18, +0x1C and +0x20, and sets +0x24 to 10.
// Returns the object pointer (the original returns `this` in eax).
static C_00840d64* __fastcall C_00840d64_Construct(C_00840d64* self, void* /*edx*/)
{
    C_00840d54_InitBase(self, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00840D64);
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 10;

    return self;
}

HOOK(0x005198C0, C_00840d64_Construct, "auto");
