#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00840ea4*, __fastcall, C_00840ea4_Construct_51C130, 0x0051C130, (C_00840ea4* self, void*));
CSF_FUNC(int, __fastcall, C_00840ec0_InitializeState, 0x0051C440, (void* self, void* edx));

static C_00840ec0* __fastcall C_00840ec0_Construct(C_00840ec0* self, void* /*edx*/)
{
    C_00840ea4_Construct_51C130(reinterpret_cast<C_00840ea4*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00840EC0);

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    self->field_0024 = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0034 = 10;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 10;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0054 = 10;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0064 = 10;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0074 = 10;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0084 = 10;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0094 = 10;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_00a4 = 10;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00b4 = 10;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;

    C_00840ec0_InitializeState(self, nullptr);
    return self;
}

HOOK(0x0051CA80, C_00840ec0_Construct, "auto");
