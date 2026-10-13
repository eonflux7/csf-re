#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00832f18_ReleaseOwnedBuffers, 0x0047E2E0, (C_00832f18* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (C_0083b958* self, void* edx));

static int __fastcall C_0083c280_Destroy(C_0083c280* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0083C280);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083C260));
    C_00832f18_ReleaseOwnedBuffers(reinterpret_cast<C_00832f18*>(&self->field_00a4), nullptr);
    CSF_AT(void*, self, 0x40) = CSF_VTBL(0x0082D410);
    return C_0083b958_Destroy(reinterpret_cast<C_0083b958*>(self), nullptr);
}

HOOK(0x004D6480, C_0083c280_Destroy, "auto");
