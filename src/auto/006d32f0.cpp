#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(void*, __fastcall, GfxFrames_InitIdentityFrame, 0x006BF1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));

static int __fastcall CCamera_Construct(CCamera* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008316B4));
    self->vtbl = CSF_VTBL(0x00860FB8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860FB0));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860F90));

    GfxObject_ConstructEmbeddedState(
        reinterpret_cast<C_00860700*>(&self->field_0010), nullptr);

    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0058 = 0;
    self->field_0068 = 0.0f;
    self->field_006c = 0.0f;
    self->field_0070 = 0.0f;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_0080 = 1065353216;
    self->field_009c = 1;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 1065353216;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00c8 = 1;

    GfxFrames_InitIdentityFrame(&self->field_0010, nullptr);

    self->field_0030 = 0;
    self->field_0044 = 0.0f;
    self->field_0048 = 0.0f;
    self->field_0050 = 0.0f;
    self->field_004c = 0;
    self->field_0064 = 0;
    self->field_000c = 0;
    self->field_00dc = 0.0f;
    self->field_00e0 = 0.0f;

    return reinterpret_cast<int>(self);
}

HOOK(0x006D32F0, CCamera_Construct, "auto");
