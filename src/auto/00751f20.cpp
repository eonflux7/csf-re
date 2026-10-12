#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, DestroySpawnMessageBase, 0x007A9C60, (void* self, void* edx));

static C_00868abc* __fastcall C_00868abc_Destroy(
    C_00868abc* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00868ABC);
    self->field_0020 = 0;
    self->field_00a8 = 0;
    DestroySpawnMessageBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x00751F20, C_00868abc_Destroy, "auto");
