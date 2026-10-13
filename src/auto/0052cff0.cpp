#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuProgressInput_ResetSlotsAndUpdate, 0x0045BD10, (void* self, void*));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static int* __fastcall C_008414e8_Construct(
    C_008414e8* self, void* /*edx*/, int parameter)
{
    *(void***)self = CSF_VTBL(0x0083042C);
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    *(void***)self = CSF_VTBL(0x008414E8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00841498));
    MenuProgressInput_ResetSlotsAndUpdate(
        reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    self->field_0130 = 0;
    self->field_0134 = 0;
    self->field_0138 = 0;
    self->field_013c = 0;
    self->field_0140 = 0;
    self->field_0144 = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->field_0150 = 0;
    self->field_0154 = 0;
    self->field_0158 = 0;

    CSF_AT(float, self, 0x160) = 0.0f;
    CSF_AT(float, self, 0x164) = 0.0f;
    CSF_AT(float, self, 0x168) = 0.0f;
    CSF_AT(float, self, 0x15C) = 1.0f;
    CSF_AT(float, self, 0x16C) = 1.0f;
    CSF_AT(float, self, 0x170) = 0.0f;
    CSF_AT(float, self, 0x174) = 0.0f;
    CSF_AT(float, self, 0x178) = 0.0f;
    CSF_AT(float, self, 0x17C) = 1.0f;
    CSF_AT(float, self, 0x180) = 0.0f;
    CSF_AT(float, self, 0x184) = 0.0f;
    CSF_AT(float, self, 0x188) = 0.0f;
    CSF_AT(float, self, 0x18C) = 1.0f;

    self->field_001c = 0;
    self->field_003c = parameter;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0014 = 0;
    self->field_0040 = 0;
    self->field_0018 = 0;
    self->vClearField0010();

    return reinterpret_cast<int*>(self);
}

HOOK(0x0052CFF0, C_008414e8_Construct, "auto");
