#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

static int __thiscall C_0082db60_ClearSceneFields(C_0082db60* self)
{
    SceneLoader_NoOp();
    self->field_0090 = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    return 0;
}

HOOK(0x0040C830, C_0082db60_ClearSceneFields, "auto");
