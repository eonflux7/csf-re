#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_ResetPanelState(CGuiPanel* self, void* /*edx*/)
{
    self->m_pBackgroundQuad = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->m_pPointerQuad = 0;
    self->m_pInitialFocus = 0;
    self->field_0188 = 1;
    self->field_0189 = 1;
    self->field_018a = 0;
    return 0;
}

HOOK(0x0072A360, CGuiPanel_ResetPanelState, "auto");
