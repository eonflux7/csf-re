#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// Returns the scene object's inactive flag if it is set; otherwise forwards to
// the class's slot-0x134 virtual handler with (0, 0) and returns its result.
static int __fastcall CCharWeaponFsm_ProcessIfSceneActive(CCharWeaponFsm* self, void* /*edx*/) {
    int result = SceneObj_IsInactive(reinterpret_cast<C_00833b88*>(self->field_0008), nullptr);
    if (!static_cast<uint8_t>(result)) {
        return self->vSlot134(0, 0);
    }
    return result;
}

HOOK(0x0047F990, CCharWeaponFsm_ProcessIfSceneActive, "auto");
