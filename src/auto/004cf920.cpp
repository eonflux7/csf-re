#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet)
CSF_FUNC(int, __fastcall, SceneObject_ResetAttachedColorOverride, 0x004CF850, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ResetLayoutDefaults, 0x004525A0, (CGuiControl* self, void* edx));

// Data
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Sets or clears bit 0x4 of the object's flags field at +0x40, then resets
// the attached color override on this object and the layout defaults of the current scene.
static int __fastcall C_0083b4a0_SetColorOverrideFlagAndReset(C_0083b4a0* self, void* /*edx*/, char enable) {
    uint32_t flags = self->field_0040;
    if (enable) {
        flags = flags | 4;
    } else {
        flags = flags & 0xFFFFFFFBu;
    }
    self->field_0040 = flags;

    SceneObject_ResetAttachedColorOverride(self, nullptr);
    return CGuiControl_ResetLayoutDefaults(reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
}

HOOK(0x004CF920, C_0083b4a0_SetColorOverrideFlagAndReset, "auto");
