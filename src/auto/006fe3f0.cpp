#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

CSF_GLOBAL(uint32_t, g_ClumpDefaultValue, 0x00861238);
CSF_GLOBAL(uint32_t, g_ClumpDefaultRangeA, 0x0086123C);
CSF_GLOBAL(uint32_t, g_ClumpDefaultRangeB, 0x00861240);

static int __fastcall C_00863044_Construct(C_00863044* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00863044);

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0004 = 957533431;
    self->field_0014 = 1097859072;
    self->field_0018 = 1097859072;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 10;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0020 = 0;
    self->field_0021 = 0;
    self->field_0022 = 1;

    return reinterpret_cast<int>(self);
}

HOOK(0x006FE3F0, C_00863044_Construct, "auto");
