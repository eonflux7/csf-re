#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiControl_Construct_71A170, 0x0071A170,
         (CGuiControl* self, void* edx, int source));

CSF_GLOBAL(std::uint32_t, g_PanelVtable, 0x00865B40);

static CGuiPanel* __fastcall CGuiPanel_ConstructFromSource(
    CGuiPanel* self, void* /*edx*/, int source)
{
    CGuiControl_Construct_71A170(self, nullptr, source);

    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_PanelVtable);
    self->m_childrenBegin = 0;
    self->m_childrenEnd = 0;
    self->field_0140 = 0;
    CSF_AT(std::uint32_t, self, 0x170) = 0;
    CSF_AT(std::uint32_t, self, 0x174) = 15;
    CSF_AT(std::uint8_t, self, 0x160) = 0;
    self->field_0158 = 0;
    self->m_pBackgroundQuad = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->m_pPointerQuad = 0;
    self->m_pInitialFocus = 0;
    self->field_018a = 0;
    self->field_0188 = 1;
    self->field_0189 = 1;
    return self;
}

HOOK(0x0072C7F0, CGuiPanel_ConstructFromSource, "auto");
