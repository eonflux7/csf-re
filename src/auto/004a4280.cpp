#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

static uint32_t* __fastcall SceneMissionSetup_ConstructObjectController(C_008344f0* self, void* /*edx*/, int a2)
{
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x008316B4);
    self->field_0008 = (uint32_t)a2;
    self->vtbl = CSF_VTBL(0x008344F0);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x008344D0);

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_001c = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;

    self->field_0014 = 0xFFFFFFFFu;
    self->field_0018 = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x24) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x28) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x34) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x38) = 0xFFFFFFFFu;

    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 10;

    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0xFFFFFFFFu;
    self->field_0048 = 0xFFFFFFFFu;
    self->field_004c = 0xFFFFFFFFu;
    self->field_0078 = 0;
    self->field_0064 = 0.0f;

    S_0041e5f0* array = reinterpret_cast<S_0041e5f0*>((uint8_t*)self + 0x50);
    uint32_t capacity = CSF_AT(uint32_t, array, 4);
    if (capacity < 10u)
        S_0041e5f0_GrowArrayCapacity(array, nullptr, (int)(10u - capacity));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004A4280, SceneMissionSetup_ConstructObjectController, "auto");
