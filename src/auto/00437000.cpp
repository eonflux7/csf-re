#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the control is marked shown and not disabled, otherwise 0.
static int __fastcall CGuiControl_IsShown(CGuiControl* self, void* /*edx*/) {
    if (self->m_bShown == 0) {
        return 0;
    }
    if (self->m_bDisabled != 0) {
        return 0;
    }
    return 1;
}

HOOK(0x00437000, CGuiControl_IsShown, "auto");
