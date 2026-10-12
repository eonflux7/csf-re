#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_InitializeClumpInstance, 0x006B3FE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (void* self, void* edx));

static int __fastcall C_00860288_ConstructClumpInstance(
    C_00860288* self, void* /*edx*/, int resource)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008316B4));
    self->vtbl = CSF_VTBL(0x00860288);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860280));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860260));

    self->field_0010 = -1;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    GfxObject_ConstructEmbeddedState(&self->field_0030, nullptr);

    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 1065353216;
    self->field_00a8 = 1065353216;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00bc = 0;
    self->field_00cc = -1;
    self->field_00d0 = 0;
    self->field_000c = resource;
    self->field_0014 = -1;

    GfxObject_InitializeClumpInstance(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x006B4330, C_00860288_ConstructClumpInstance, "auto");
