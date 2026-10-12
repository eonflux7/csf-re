#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e520_Initialize, 0x0041E0F0, (C_0082e520* self, void*));

static uint32_t* __fastcall C_0082e090_Initialize(C_0082e090* self, void* /*edx*/)
{
    C_0082e520_Initialize(reinterpret_cast<C_0082e520*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0082E090);

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_003c = 1065353216;
    self->field_0058 = 1;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0065 = 0;
    self->field_0084 = 0.0f;
    self->field_0088 = 0.0f;
    self->field_008c = 0.0f;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;

    CSF_AT(void*, self, 0xAC) = CSF_VTBL(0x0082D448);
    self->field_00b0 = -1;
    self->field_00b4 = -1;
    self->field_0075 = 0;
    self->field_0076 = 0;
    self->field_0077 = 0;
    self->field_007c = 0;
    self->field_0080 = 0.0f;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_0074 = 0;
    self->field_0090 = 0;
    self->field_00b8 = 0.0f;
    self->field_0078 = 1;
    self->field_00a8 = 0.2f;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00416530, C_0082e090_Initialize, "auto");
