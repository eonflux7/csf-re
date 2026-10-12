#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetSpawnMessages_InitializeBase, 0x007A9D70, (int a1, int a2));

static uint32_t* __fastcall C_00868abc_Construct(C_00868abc* self, void* /*edx*/)
{
    NetSpawnMessages_InitializeBase(
        static_cast<int>(reinterpret_cast<uintptr_t>(self)), 0);

    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_00a8 = 0;
    self->vtbl = CSF_VTBL(0x00868ABC);
    self->field_00ac = 1;
    self->field_0028 = -1;
    self->field_002c = -1;
    self->field_0030 = -1;
    self->field_0034 = -1;
    self->field_0038 = -1;
    self->field_003c = -1;
    self->field_0040 = -1;
    self->field_0044 = -1;
    self->field_0048 = -1;
    self->field_004c = -1;
    self->field_0050 = -1;
    self->field_0054 = -1;
    self->field_0058 = -1;
    self->field_005c = -1;
    self->field_0060 = -1;
    self->field_0064 = -1;
    self->field_0068 = -1;
    self->field_006c = -1;
    self->field_0070 = -1;
    self->field_0074 = -1;
    self->field_0078 = -1;
    self->field_007c = -1;
    self->field_0080 = -1;
    self->field_0084 = -1;
    self->field_0088 = -1;
    self->field_008c = -1;
    self->field_0090 = -1;
    self->field_0094 = -1;
    self->field_0098 = -1;
    self->field_009c = -1;
    self->field_00a0 = -1;
    self->field_00a4 = -1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00751E10, C_00868abc_Construct, "auto");
