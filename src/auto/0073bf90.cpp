#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, NetSpawnMessages_DestroyBase, 0x0064EBC0, (void* self, void* edx));

static int __fastcall C_008675c8_Destroy(void* self, void* /*edx*/)
{
    // Sub-object at +0x80 (C_00867890) gets its own vtable first.
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x008675C8);
    void* subObject = static_cast<uint8_t*>(self) + 0x80;
    CSF_AT(void*, subObject, 0x00) = CSF_VTBL(0x008675B8);
    C_00867890_ResetVtableAndClearField4(static_cast<C_00867890*>(subObject), nullptr);

    // Base-class vtable (C_00867578) is installed before the member teardown.
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00867578);
    SceneLoader_NoOp();                                                // +0x68
    SceneLoader_NoOp();                                                // +0x5C
    MpSpawnPoints_FinishEventDescriptor();                             // +0x50
    SceneLoader_NoOp();                                                // +0x4A
    SceneLoader_NoOp();                                                // +0x47
    SceneLoader_NoOp();                                                // +0x44

    // Tail jump to base destroy: result is returned directly.
    return NetSpawnMessages_DestroyBase(self, nullptr);
}

HOOK(0x0073BF90, C_008675c8_Destroy, "auto");
