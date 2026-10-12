#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0082de40 (deleting variant when bit 0 of flags is set).
static C_0082de40* __fastcall C_0082de40_Destroy(C_0082de40* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x0082DE40);
    C_0082deb8_ReleaseChildBase_41E110(static_cast<C_0082e520*>(self), nullptr);
    if (self->field_0010) {
        Mem_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }
    self->vtbl = CSF_VTBL(0x0082D410);
    if ((flags & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x40F7F0, C_0082de40_Destroy, "auto");
