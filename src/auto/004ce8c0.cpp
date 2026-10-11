#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));

static C_0083ad48* __fastcall C_0083ad48_Construct(C_0083ad48* self, void* /*edx*/, int a2) {
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);
    self->field_004c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0.0f;
    self->field_0048 = 0.0f;
    self->vtbl = CSF_VTBL(0x0083AD48);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083AD28));
    return self;
}

HOOK(0x004CE8C0, C_0083ad48_Construct, "auto");
