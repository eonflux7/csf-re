#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, VecGrow_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));

static unsigned int* __fastcall C_008600d4_InitializeBase(
    C_008600d4* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x008600D4);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 10;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;

    // Also listed as a vtable reference in the packet.
    (void)CSF_VTBL(0x0041E5F0);

    uint32_t capacity = self->field_0008;
    if (capacity < 0xA)
        VecGrow_4DE9B0(reinterpret_cast<uint8_t*>(self) + 0x04,
                      nullptr, static_cast<int>(10 - capacity));

    capacity = self->field_0018;
    if (capacity < 0xA)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x14),
            nullptr, static_cast<int>(10 - capacity));

    capacity = self->field_0028;
    if (capacity < 0xA)
        VecGrow_4DE9B0(reinterpret_cast<uint8_t*>(self) + 0x24,
                      nullptr, static_cast<int>(10 - capacity));

    capacity = self->field_0038;
    if (capacity < 0x64)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x34),
            nullptr, static_cast<int>(100 - capacity));

    return reinterpret_cast<unsigned int*>(self);
}

HOOK(0x006AF390, C_008600d4_InitializeBase, "auto");
