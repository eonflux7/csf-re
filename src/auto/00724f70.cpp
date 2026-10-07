#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// CGuiButton::SetPermanentTime(float time)
// Slot that configures the fourth state record (PERMANENTE): marks it as timed,
// stores the duration and enables per-state time evaluation. Always returns 1.
static char __fastcall CGuiButton_SetPermanentTime(CGuiButton* self, void* /*edx*/, float time)
{
    self->m_bPermanentTimed = 1;
    self->m_permanentTime = time;
    self->m_bStateTimes = 1;
    return 1;
}

HOOK(0x00724F70, CGuiButton_SetPermanentTime, "auto");
