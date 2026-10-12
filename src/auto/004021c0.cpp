#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void* edx));

static float* __fastcall HudAlertIcons_BuildQueryContext(C_00833b88* self, void* /*edx*/,
                                                         uint32_t* a2, uint32_t* a3) {
    CSF_AT(uint32_t, self, 0x0) = a3[0];
    self->field_0004 = a3[1];
    CSF_AT(uint32_t, self, 0x8) = a3[2];
    self->field_000c = a2[0];
    CSF_AT(uint32_t, self, 0x10) = a2[1];
    CSF_AT(uint32_t, self, 0x14) = a2[2];
    return NormalizeVector3(self, nullptr);
}

HOOK(0x004021C0, HudAlertIcons_BuildQueryContext, "auto");
