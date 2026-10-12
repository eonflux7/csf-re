#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Base_0082de40_Construct, 0x0040F720, (void* self, void* edx));

static uint32_t* __fastcall C_0082dc88_Construct(C_0082dc88* self, void* /*edx*/, char a2)
{
    Base_0082de40_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x0082DC88);
    self->field_0034 = 0;
    self->field_0038 = static_cast<uint8_t>(a2);
    self->field_0030 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0048 = 0.0f;
    self->field_004c = 0;
    self->field_0034 = 0;
    self->field_0039 = 1;
    self->field_0034 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040D300, C_0082dc88_Construct, "auto");
