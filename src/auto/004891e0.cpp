#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_481E80, 0x00481E80, (void* self, void* edx, uint32_t* a2));

static int __fastcall CCharActionFsm_Construct(
    CCharActionFsm* self, void* /*edx*/, uint32_t* owner)
{
    sub_481E80(self, nullptr, owner);

    self->vtbl = CSF_VTBL(0x008334F0);
    self->field_0004 =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008334D0));
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    CSF_AT(void*, self, 0xB0) = CSF_VTBL(0x0082D448);
    self->field_00b4 = -1;
    self->field_00b8 = -1;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00bc = 0;
    self->field_00bd = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00d4 = 0;
    self->field_00c8 = -1082130432;
    self->field_00cc = -1.0f;
    self->field_00d0 = -1.0f;
    self->field_0090 = -1;
    self->field_0094 = -1;
    self->field_0098 = -1;
    self->field_005f = 1;

    return reinterpret_cast<int>(self);
}

HOOK(0x004891E0, CCharActionFsm_Construct, "auto");
