#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_Reset(CGuiControl* self, void* /*edx*/)
{
    ((void(__fastcall*)(void*, void*))self->vtbl[0x16C / 4])(self, nullptr);

    self->m_bDraggable = 0;
    self->m_bDisabled = 0;
    self->m_bShown = 1;
    self->field_0015 = 0;
    self->m_pParent = 0;
    self->m_bFocused = 0;
    self->m_bSelected = 0;

    self->m_colorR = 1.0f;
    self->m_colorG = 1.0f;
    self->m_colorB = 1.0f;
    self->m_colorA = 1.0f;

    self->m_colorAnimTime = 0.0f;
    self->m_pColorAnim = 0;
    self->m_posAnimTime = 0.0f;
    self->m_pPosAnim = 0;
    self->m_sizeAnimTime = 0.0f;
    self->m_pSizeAnim = 0;

    self->m_bFiringEvent = 0;
    self->m_pRenderer = 0;
    self->m_pDebugRect = 0;
    self->field_0118 = 0;
    self->field_011c = 0;
    self->field_0120 = 0;
    self->field_0124 = 0;

    return 0;
}

HOOK(0x00718CC0, CGuiControl_Reset, "auto");
