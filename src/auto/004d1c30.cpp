#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (C_0083b958* self, void* , int a2));

static int __fastcall C_0083ba40_Construct(C_0083ba40* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(static_cast<C_0083b958*>(self), nullptr, a2);
    self->vtbl = CSF_VTBL(0x0083BA40);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083BA24);

    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0041 = 1;
    self->field_0060 = 1;
    self->field_0062 = 1;
    self->field_0040 = 0;
    self->field_0042 = 0;
    self->field_0043 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0061 = 0;
    self->field_0063 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0.0f;
    self->field_00a0 = 0;
    self->field_0065 = 0;
    self->field_0066 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00a4 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0067 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0064 = static_cast<uint8_t>(-1);
    self->field_008c = static_cast<uint32_t>(-1);
    self->field_0090 = static_cast<uint32_t>(-1);

    return reinterpret_cast<int>(self);
}

HOOK(0x004D1C30, C_0083ba40_Construct, "auto");
