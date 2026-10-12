#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, MenuSystem_GetIndexedGlobalFloat, 0x0050DA90, (void* self, void*));

static float* __fastcall C_0083114c_Construct(C_0083114c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083114C);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 10;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0028 = 0;
    self->field_0024 = 0;
    self->field_0058 = 0;
    self->field_005c = 1045220557;

    const int globalState = GetGlobalState_50D190();
    self->field_0060 = static_cast<float>(
        MenuSystem_GetIndexedGlobalFloat(
            reinterpret_cast<void*>(globalState), nullptr));
    return reinterpret_cast<float*>(self);
}

HOOK(0x0044A420, C_0083114c_Construct, "auto");
