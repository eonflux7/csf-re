#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(float*, __fastcall, BaseObject_Construct, 0x004E64F0, (void* self, void* edx, int a2, float a3, int a4, int a5));

static int __fastcall C_0083eb70_Construct(C_0083eb70* self, void* /*edx*/, int a2, float a3, int a4, int a5)
{
    BaseObject_Construct(self, nullptr, a2, a3, a4, a5);

    self->vtbl = CSF_VTBL(0x0083EB70);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083EB54);

    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 10;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 10;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 10;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 10;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 10;
    self->field_00a0 = 0.0f;

    uint32_t count = self->field_0054;
    if (count < 10)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0050), nullptr, 10 - count);

    count = self->field_0064;
    if (count < 10)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0060), nullptr, 10 - count);

    count = self->field_0074;
    if (count < 10)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0070), nullptr, 10 - count);

    count = self->field_0084;
    if (count < 10)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0080), nullptr, 10 - count);

    count = self->field_0094;
    if (count < 10)
        S_00413570_GrowEventRecords(reinterpret_cast<S_00413570*>(&self->field_0090), nullptr, 10 - count);

    return (int)(intptr_t)self;
}

HOOK(0x004E86B0, C_0083eb70_Construct, "auto");
