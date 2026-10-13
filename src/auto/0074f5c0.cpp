#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));

// Destructor-style teardown for C_00868768: installs its vtable, runs the
// no-op scene loader hook on the embedded member at +0x44, then tail-jumps
// into the base destroy routine with the same object.
static int __fastcall C_00868768_Destroy(C_00868768* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00868768);
    SceneLoader_NoOp();
    // Tail jump: ecx = this, edx and the (empty) stack arguments are forwarded.
    return NetSpawnMessages_DestroyBase(self, nullptr);
}

HOOK(0x0074F5C0, C_00868768_Destroy, "auto");
