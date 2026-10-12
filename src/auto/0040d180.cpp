#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bound by address: original calls SceneLoader_NoOp (0x506AC0), __cdecl, no args.
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// __thiscall method of C_0082dc88, ret 0.
static int __fastcall C_0082dc88_ResetAndMarkReady(C_0082dc88* self, void* /*edx*/)
{
    SceneLoader_NoOp();
    self->field_0039 = 1;
    self->field_0034 = 0;
    self->field_0034 |= 1u;
    return 1;
}

HOOK(0x0040D180, C_0082dc88_ResetAndMarkReady, "auto");
