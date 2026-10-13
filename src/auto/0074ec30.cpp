#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));

// Original: stores C_00868658's vtable, runs three no-op scene-loader hooks on
// embedded sub-objects (the cdecl callee ignores its ecx), then tail-jumps to
// the base destroy routine with ecx = this.
static int __fastcall C_00868658_ResetVtableAndDestroyBase(C_00868658* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00868658);

    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();

    return NetSpawnMessages_DestroyBase(self, nullptr);
}

HOOK(0x0074EC30, C_00868658_ResetVtableAndDestroyBase, "auto");
