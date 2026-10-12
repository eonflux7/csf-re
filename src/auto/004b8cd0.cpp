#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

static int __fastcall S_004ad990_InitializeFromCombatState(
    S_004ad990* self, void* /*edx*/, const S_004ad990* source)
{
    self->field_0028 = source->field_0028;
    self->field_002c = source->field_002c;
    self->field_0030 = source->field_0030;
    self->field_0038 = source->field_0038;
    self->field_003c = source->field_003c;
    self->field_0040 = source->field_0040;
    CSF_AT(float, self, 0x44) = CSF_AT(float, source, 0x44);
    CSF_AT(float, self, 0x48) = CSF_AT(float, source, 0x48);
    self->field_0050 = -1;
    self->field_0054 = -1;

    uint32_t* objectPosition = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    self->field_005c = 0;
    CSF_AT(float, self, 0x70) = self->field_0040;
    CSF_AT(float, self, 0x74) = CSF_AT(float, self, 0x44);
    CSF_AT(float, self, 0x78) = CSF_AT(float, self, 0x48);
    self->field_0060 = 1065353216;
    CSF_AT(uint32_t, self, 0x64) = objectPosition[0];
    CSF_AT(uint32_t, self, 0x68) = objectPosition[1];
    CSF_AT(uint32_t, self, 0x6C) = objectPosition[2];

    self->field_0098 = source->field_0098;
    self->field_009c = source->field_009c;
    self->field_00a8 = source->field_00a8;
    self->field_00a9 = 0;
    self->field_00a4 = 0;
    return 0;
}

HOOK(0x004B8CD0, S_004ad990_InitializeFromCombatState, "auto");
