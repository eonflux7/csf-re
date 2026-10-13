#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets the selected flag of a GUI control. A selected state is only honored
// when the control is selectable; otherwise the stored selected flag is kept.
static char __fastcall CGuiControl_SetSelected(CGuiControl* self, void* /*edx*/, char wantSelected)
{
    char result = wantSelected;
    if (!self->m_bSelectable && wantSelected) {
        result = self->m_bSelected;
    }
    self->m_bSelected = result;
    return result;
}

HOOK(0x437020, CGuiControl_SetSelected, "auto");
