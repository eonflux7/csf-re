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

// Callee: returns nonzero when the scene object is inactive (tested as a byte in the original).
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// Callee: tail-jumped to by the original; forwards the same `this` in ecx.
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateActiveState, 0x004849A0, (void* self, void* edx));

// Original 0x00484E40: if the linked scene object (field +0x008) is not inactive, notify it
// through vtable slot 0x94 and continue with the action update; otherwise return the inactive flag.
static int __fastcall CCharActionFsm_UpdateIfSceneObjActive(void* self, void* /*edx*/)
{
    // Scene object pointer stored at this+0x008 (CCharWeaponFsm::field_0008).
    void* sceneObj = CSF_AT(void*, self, 0x008);

    int result = SceneObj_IsInactive(sceneObj, nullptr);
    if (!static_cast<uint8_t>(result)) {
        // Virtual slot 0x94 (byte offset) -> index 0x94 / 4 = 37, called with no stack args.
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, sceneObj, 0)[0x94 / 4])(sceneObj, nullptr);
        return CCharActionFsm_UpdateActiveState(self, nullptr);
    }
    return result;
}

HOOK(0x00484E40, CCharActionFsm_UpdateIfSceneObjActive, "auto");
