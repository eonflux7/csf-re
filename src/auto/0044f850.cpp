#include <cstdint>
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Returns non-zero unless the control's scene-slot field holds the bit pattern of -1.0f.
static int __fastcall CGuiControl_IsSceneActive(CGuiControl* self, void* /*edx*/) {
    return self->field_0120 != 0xBF800000u ? 1 : 0;
}

HOOK(0x0044F850, CGuiControl_IsSceneActive, "auto");
