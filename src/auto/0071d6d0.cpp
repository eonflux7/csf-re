#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, const char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, ReserveFirstCollection, 0x006DAD80,
         (void* self, void* edx, unsigned int capacity));
CSF_FUNC(unsigned int, __fastcall, ReserveSecondCollection, 0x0071BCC0,
         (void* self, void* edx, unsigned int capacity));
CSF_FUNC(unsigned int, __fastcall, ReserveObjectPointers, 0x0050CD00,
         (void* self, void* edx, unsigned int capacity));


static C_00864508* __fastcall C_00864508_Initialize(
    C_00864508* self, void* /*edx*/, void* owner,
    uint32_t firstParameter, uint32_t secondParameter)
{
    C_00830438_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00864508);

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0094 = 15;
    self->field_0090 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0080) = 0;
    self->field_000c = reinterpret_cast<uint32_t>(owner);

    void* settings = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr);
    self->field_0070 = CSF_AT(uint32_t, settings, 0x18);

    settings = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr);
    const float initialValue = CSF_AT(float, settings, 0x1C);
    self->field_0074 = initialValue;
    self->field_0078 = initialValue;
    self->field_0068 = secondParameter;
    self->field_0064 = firstParameter;

    void* manager = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[0x14 / 4])(owner, nullptr);
    self->field_0040 = reinterpret_cast<uint32_t>(manager);

    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, manager, 0)[0x240 / 4])(manager, nullptr, 1);

    uint32_t parameters[2] = { firstParameter, secondParameter };
    manager = reinterpret_cast<void*>(self->field_0040);
    ((void (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, manager, 0)[0xA0 / 4])(manager, nullptr, parameters);

    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x7C,
        nullptr, CSF_STR(0x0082E65C, ""), 0);

    self->field_006c = 0;
    self->field_0098 = 0;
    self->field_009c = 0.0f;
    self->field_00a0 = 0;
    self->field_00a1 = 1;
    self->field_00a4 = 0;

    ReserveFirstCollection(reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, 5);
    ReserveSecondCollection(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr, 5);
    ReserveObjectPointers(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr, 5);
    ReserveObjectPointers(reinterpret_cast<uint8_t*>(self) + 0x54, nullptr, 5);
    return self;
}

HOOK(0x0071D6D0, C_00864508_Initialize, "auto");
