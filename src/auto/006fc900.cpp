#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of C_00862e88 (fastcall, ecx = this, edx unused).
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Matrix allocator used by the light object (cdecl, no arguments).
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// CLightObject constructor: builds the object in place and returns this.
static int __fastcall C_00862e88_Construct(C_00862e88* self, void* /*edx*/) {
    C_00860178_Construct(self, nullptr);

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    CSF_AT(std::uint32_t, self, 0x8) = CSF_ADDR_DATA(0x008316B4);
    self->vtbl = CSF_VTBL(0x00862E88);
    CSF_AT(std::uint32_t, self, 0x4) = CSF_ADDR_DATA(0x00862E7C);
    CSF_AT(std::uint32_t, self, 0x8) = CSF_ADDR_DATA(0x00862E5C);

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 1065353216;
    self->field_0038 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(RwMatrixCreate()));
    self->field_003c = 1;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 10;
    self->field_0010 = 0;
    self->field_0054 = 0;
    self->field_0034 = 0.0f;

    return reinterpret_cast<int>(self);
}

HOOK(0x006FC900, C_00862e88_Construct, "auto");
