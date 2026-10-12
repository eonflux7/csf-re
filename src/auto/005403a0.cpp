#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_004f21d0_ResetSaveData, 0x005166E0, (S_004f21d0* self, void*));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static uint32_t* __fastcall CMenuMissionInfo_Construct(
    CMenuMissionInfo* self, void* /*edx*/, int context)
{
    self->vtbl = CSF_VTBL(0x0083042C);
    C_00830438_Construct(
        reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);

    self->vtbl = CSF_VTBL(0x00842E08);
    self->field_0004 =
        reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x00842DB8));

    self->field_0048 = 7;
    self->field_0044 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0034) = 0;

    CSF_AT(int32_t, self, 0x5C) = -1;
    CSF_AT(int32_t, self, 0x58) = -1;
    CSF_AT(int32_t, self, 0x54) = -1;
    CSF_AT(int32_t, self, 0x50) = -1;
    CSF_AT(int32_t, self, 0x4C) = -1;
    CSF_AT(uint32_t, self, 0x60) = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;

    S_004f21d0_ResetSaveData(
        reinterpret_cast<S_004f21d0*>(
            reinterpret_cast<uint8_t*>(self) + 0x30),
        nullptr);

    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 10;
    self->field_0018 = context;
    self->field_001c = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0014 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0010 = 0;
    self->field_0080 = 0;
    self->field_00a4 = 0;
    self->field_00a5 = 0;
    self->field_00a6 = 0;
    self->field_00a7 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005403A0, CMenuMissionInfo_Construct, "auto");
