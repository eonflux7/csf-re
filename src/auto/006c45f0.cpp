#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor C_00860178 (0x6B2820), __thiscall, no stack args.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// C_00860894 constructor: runs the base constructor, stores the two ctor
// arguments, zeroes the secondary fields and installs C_00860894's vtable.
static int __fastcall C_00860894_Init(C_00860894* self, void* /*edx*/, int a2, int a3)
{
    C_00860178_Construct(self, nullptr);
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0008 = 0;
    self->field_0010 = 0;
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x00860894);
    self->field_000c = static_cast<uint32_t>(a3);
    return reinterpret_cast<int>(self);
}

HOOK(0x006C45F0, C_00860894_Init, "auto");
