#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

static int __fastcall C_0082dd78_InitSceneLoaderState(C_0082dd78* self, void* /*edx*/) {
    SceneLoader_NoOp();
    self->field_0039 = 1;
    self->field_0070 |= 0x20u;
    self->field_0034 = 0;
    self->field_0034 |= 1u;
    return 1;
}

HOOK(0x0040E150, C_0082dd78_InitSceneLoaderState, "auto");
