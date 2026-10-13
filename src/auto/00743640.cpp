#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address with the signatures from the packet.
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));

// Sets the object's vtable, releases the embedded sub-objects in the original order,
// then tail-jumps into the base destructor chain with the same `this`.
static int __fastcall C_00867ab8_ReleaseCallbacksAndDestroyBase(C_00867ab8* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867AB8);

    SceneLoader_NoOp();                                                      // this + 0xA0
    SceneLoader_NoOp();                                                      // this + 0x88
    SceneLoader_NoOp();                                                      // this + 0x78
    SceneLoader_NoOp();                                                      // this + 0x6C
    MpSpawnPoints_FinishEventDescriptor();                                   // this + 0x5C
    SceneLoader_NoOp();                                                      // this + 0x50
    SceneLoader_NoOp();                                                      // this + 0x44

    return NetSpawnMessages_DestroyBase(self, nullptr);
}

HOOK(0x00743640, C_00867ab8_ReleaseCallbacksAndDestroyBase, "auto");
