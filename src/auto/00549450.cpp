#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static int* __fastcall CMenuVideoAdvanced_Construct(
    CMenuVideoAdvanced* self, void* /*edx*/, int argument)
{
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0083042C));
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00843FB0));
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00844000));

    self->field_0048 = argument;
    self->field_001c = 0;

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    uint32_t context = CSF_AT(uint32_t, globalObject, 0x18);
    self->field_0020 = context;
    uint32_t contextMember = context
        ? CSF_AT(uint32_t, reinterpret_cast<void*>(context), 0x0C)
        : 0;

    self->field_0024 = contextMember;
    self->field_0050 = 0;
    self->field_0014 = 0;
    self->field_004c = 0;
    self->field_0018 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;

    self->vClearField0010();
    return reinterpret_cast<int*>(self);
}

HOOK(0x00549450, CMenuVideoAdvanced_Construct, "auto");
