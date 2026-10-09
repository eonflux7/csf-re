#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ResetSecondManager, 0x005258F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_00841090 (deleting variant when bit 0 of flags is set).
static C_00841090* __fastcall C_00841090_Destroy(C_00841090* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00841090);
    SceneLoader_ResetSecondManager(self, nullptr);
    if (self->field_008c) {
        Mem_Free(reinterpret_cast<void*>(self->field_008c));
        self->field_008c = 0;
    }
    self->vtbl = CSF_VTBL(0x0082D410);
    if ((flags & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x525AE0, C_00841090_Destroy, "auto");
