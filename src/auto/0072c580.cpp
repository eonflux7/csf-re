#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, ConstructGuiControl, 0x0071A010,
         (CGuiControl* self, void* edx, int argument1, int argument2));
CSF_FUNC(legacy::string*, __fastcall, AssignStringRange, 0x00429B60,
         (legacy::string* self, void* edx,
          const char* text, unsigned int length));

CSF_GLOBAL(std::uint32_t, g_PanelVtable, 0x00865B40);

static CGuiPanel* __fastcall CGuiPanel_Construct(
    CGuiPanel* self, void* /*edx*/, int argument1, int argument2)
{
    ConstructGuiControl(self, nullptr, argument1, argument2);

    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_PanelVtable);
    self->field_0138 = 0;
    self->field_013c = 0;
    self->field_0140 = 0;

    CSF_AT(std::uint32_t, self, 0x174) = 15;
    CSF_AT(std::uint32_t, self, 0x170) = 0;
    CSF_AT(std::uint8_t, self, 0x160) = 0;
    self->field_0158 = 0;
    AssignStringRange(
        &self->m_str15C, nullptr, CSF_STR(0x0082E65C, ""), 0);

    self->field_0188 = 1;
    self->field_0189 = 1;
    self->m_pBackgroundQuad = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->m_pPointerQuad = 0;
    self->m_pInitialFocus = 0;
    self->field_018a = 0;
    return self;
}

HOOK(0x0072C580, CGuiPanel_Construct, "auto");
