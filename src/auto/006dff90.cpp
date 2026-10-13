#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(void*, __fastcall, RecordArray_Grow_6D9EB0, 0x006D9EB0, (void* self, void* edx, int a2));

static int __fastcall C_00861988_Construct(C_00861988* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x008304D8);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861988);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00861980);

    self->field_0008 = 100;
    self->field_000c = 1;

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;

    self->field_0024 = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    self->field_002c = 0;
    self->field_0030 = 10;

    self->field_0034 = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    self->field_003c = 0;
    self->field_0040 = 10;

    self->field_0050 = 0;
    self->field_0054 = 0;

    // The packet also lists this callable address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t secondCapacity = CSF_AT(uint32_t, self, 0x28);
    if (secondCapacity < 0x14)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x24),
            nullptr, static_cast<int>(20 - secondCapacity));
    }
    self->field_0030 = 10;

    const uint32_t firstCapacity = self->field_0018;
    if (firstCapacity < 0x32)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x14),
            nullptr, static_cast<int>(50 - firstCapacity));
    }
    self->field_0020 = 30;

    const uint32_t recordCapacity = CSF_AT(uint32_t, self, 0x38);
    if (recordCapacity < 5)
    {
        RecordArray_Grow_6D9EB0(
            reinterpret_cast<uint8_t*>(self) + 0x34,
            nullptr, static_cast<int>(5 - recordCapacity));
    }

    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_005c = 0;
    self->field_0040 = 5;
    self->field_004c = 1;
    self->field_0010 = 0;
    self->field_0058 = 1;
    self->field_0060 = 2;

    return reinterpret_cast<int>(self);
}

HOOK(0x006DFF90, C_00861988_Construct, "auto");
