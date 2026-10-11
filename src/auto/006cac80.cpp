#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: GetSceneRegionAt (__fastcall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));

// Returns the scene region for the argument, looked up through the scene object
// stored at +0x0BC. The original tail-jumps to GetSceneRegionAt, so the
// callee's ret 4 pops a2 and this wrapper pops the same 4 bytes.
static int __fastcall C_00860bb8_LookupSceneRegion(C_00860bb8* self, void* /*edx*/, int a2)
{
    C_0085e7b8* sceneObject = reinterpret_cast<C_0085e7b8*>(self->field_00bc);
    return GetSceneRegionAt(sceneObject, nullptr, a2);
}

HOOK(0x006CAC80, C_00860bb8_LookupSceneRegion, "auto");
