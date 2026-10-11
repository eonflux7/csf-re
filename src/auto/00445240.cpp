#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_ReleaseSecondaryResources, 0x00444FD0,
         (void* self, void* edx));

static char __fastcall S_00444fd0_ClearSecondaryResources(
    S_00444fd0* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x00444FD0);

    if (self->field_0004)
        self->field_0004 = 0;

    SceneLoader_ReleaseSecondaryResources(self, nullptr);
    self->field_0004 = 0;
    return 0;
}

HOOK(0x00445240, S_00444fd0_ClearSecondaryResources, "auto");
