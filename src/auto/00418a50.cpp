#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor chain step (C_0082de40), called first with this in ecx.
CSF_FUNC(uint32_t*, __fastcall, Base_0082de40_Construct, 0x0040F720, (void* self, void* edx));

// Constructor of the wheel-type vehicle component (C_0082e1e8).
// Original: __thiscall, one stack arg (char a2), ret 4.
static int __fastcall C_0082e1e8_Construct(C_0082e1e8* self, void* /*edx*/, char a2)
{
    Base_0082de40_Construct(self, nullptr);

    self->vtbl = CSF_VTBL(0x0082E1E8);

    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0060 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_007c = 0;
    self->field_0084 = static_cast<uint8_t>(a2);
    self->field_0030 = 0;
    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->field_006c = 0.0f;
    self->field_0070 = 0.0f;
    self->field_0074 = 0.0f;
    self->field_0078 = 0.0f;
    self->field_0088 = 0;
    self->field_007c = 0;
    self->field_0085 = 1;
    self->field_0086 = 1;
    self->field_007c = 1;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x418A50, C_0082e1e8_Construct, "auto");
