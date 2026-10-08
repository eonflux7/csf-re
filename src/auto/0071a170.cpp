#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, C_00830438_Construct, 0x00712CC0,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Reset, 0x00718CC0,
         (CGuiControl* self, void* edx));

CSF_GLOBAL(std::uint32_t, g_GuiControlVtable, 0x00864288);

static int __fastcall CGuiControl_Construct(
    CGuiControl* self, void* /*edx*/, int owner)
{
    C_00830438_Construct(self, nullptr);

    self->field_0010 = static_cast<std::uint32_t>(owner);
    CSF_AT(std::uint32_t, self, 0x00) =
        reinterpret_cast<std::uint32_t>(&g_GuiControlVtable);

    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 15;
    CSF_AT(std::uint8_t, self, 0x1C) = 0;

    CSF_AT(std::uint32_t, self, 0x4C) = 15;
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    CSF_AT(std::uint8_t, self, 0x38) = 0;

    CSF_AT(std::uint32_t, self, 0x68) = 15;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint8_t, self, 0x54) = 0;

    CSF_AT(std::uint32_t, self, 0x84) = 15;
    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(std::uint8_t, self, 0x70) = 0;

    CSF_AT(std::uint32_t, self, 0xA0) = 15;
    CSF_AT(std::uint32_t, self, 0x9C) = 0;
    CSF_AT(std::uint8_t, self, 0x8C) = 0;

    self->m_x = 0;
    self->m_y = 0;
    self->m_width = 0;
    self->m_height = 0;
    self->m_colorR = 0.0f;
    self->m_colorG = 0.0f;
    self->m_colorB = 0.0f;
    self->m_colorA = 1.0f;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->m_listenersBegin = 0;
    self->m_listenersEnd = 0;
    self->m_listenersCap = 0;
    self->m_bDebugRect = 0;
    self->m_pEffects = 0;
    self->m_bSelectable = 1;
    self->field_0130 = 0;

    CGuiControl_Reset(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x0071A170, CGuiControl_Construct, "auto");
