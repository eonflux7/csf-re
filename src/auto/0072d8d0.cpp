#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiControl_Construct_71A010, 0x0071A010,
         (CGuiSlider* self, void* edx, int argument2, int argument3));

CSF_GLOBAL(void*, g_CGuiSliderVtable, 0x00865E38);

static CGuiSlider* __fastcall CGuiSlider_Construct(
    CGuiSlider* self, void* /*edx*/, int argument2, int argument3)
{
    CGuiControl_Construct_71A010(self, nullptr, argument2, argument3);

    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_CGuiSliderVtable);

    self->field_0158 = 0;
    self->field_015c = 0;
    self->field_0160 = 0;
    self->field_0164 = 0;
    self->m_pArrowUp = 0;
    self->m_pArrowDown = 0;
    self->m_pThumb = 0;
    self->m_bHorizontal = 0;
    self->m_pos = 0;
    self->m_pageSize = 0;
    self->m_pSelectedPart = 0;
    self->field_0138 = 0.0f;
    self->field_0168 = 0;
    self->m_bConsole = 0;
    self->m_bWrap = 0;
    self->m_count = 100;
    self->field_016a = 1;

    return self;
}

HOOK(0x0072D8D0, CGuiSlider_Construct, "auto");
