#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* edx, int additionalCapacity));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_DegToRad, 0x0083E9A8);

static C_0083e9d0* __fastcall C_0083e9d0_Construct(C_0083e9d0* self, void* /*edx*/, int a2, float a3, int a4, int a5)
{
    float halfAngle = a3 * g_HalfDurationScale;

    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x0082D428);
    self->vtbl = CSF_VTBL(0x0083E9D0);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x0083E9B0);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_002c = 10;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_003c = halfAngle;
    self->field_0008 = (uint32_t)a2;
    self->field_004c = 1;
    self->field_0030 = 1;
    self->field_004d = 1;
    self->field_0034 = (uint32_t)a4;
    self->field_0038 = (uint32_t)a5;
    self->field_0044 = 0xFFFFFFFFu;
    self->field_0048 = 0.0f;
    self->field_001c = 0;
    self->field_0040 = (float)cos(halfAngle * g_DegToRad);

    unsigned int capacity = self->field_0010;
    if (capacity < 5)
        S_00413570_GrowEventRecords(reinterpret_cast<S_00413570*>(&self->field_000c), nullptr, (int)(5 - capacity));
    self->field_0018 = 5;
    return self;
}

HOOK(0x004E64F0, C_0083e9d0_Construct, "auto");
