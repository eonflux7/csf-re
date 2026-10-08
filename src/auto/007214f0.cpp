#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_Ctor, 0x0071A010,
         (CGuiControl* self, void* edx, int parent, int argument));
CSF_FUNC(int, __fastcall, CGuiEdit_InitializeState, 0x007213C0,
         (CGuiEdit* self, void* edx));

CSF_GLOBAL(std::uint32_t, g_GuiEditVtable, 0x00864E18);

static int __fastcall CGuiEdit_Construct(
    CGuiEdit* self, void* /*edx*/, int parent, int argument)
{
    CGuiControl_Ctor(reinterpret_cast<CGuiControl*>(self), nullptr,
                     parent, argument);

    CSF_AT(std::uint32_t, self, 0) =
        reinterpret_cast<std::uint32_t>(&g_GuiEditVtable);

    CSF_AT(std::uint32_t, self, 0x164) = 0;
    CSF_AT(std::uint32_t, self, 0x168) = 7;
    CSF_AT(std::uint16_t, self, 0x154) = 0;

    CSF_AT(std::uint32_t, self, 0x184) = 7;
    CSF_AT(std::uint32_t, self, 0x180) = 0;
    CSF_AT(std::uint16_t, self, 0x170) = 0;

    CSF_AT(std::uint32_t, self, 0x1A0) = 7;
    CSF_AT(std::uint32_t, self, 0x19C) = 0;
    CSF_AT(std::uint16_t, self, 0x18C) = 0;

    self->field_01c6 = 1;
    self->field_01c7 = 1;
    self->field_013c = 0;
    self->m_blinkTime = 0.5f;
    self->field_0144 = 0.0f;
    self->field_0148 = 0;
    self->field_0149 = 0;
    self->m_maxLength = 15;
    self->field_014a = 0;
    self->field_014c = 1.0f;
    self->field_01b4 = 0xFFFFFFFFu;
    self->field_01b8 = 0.0f;
    self->field_01bc = 0xFFFFFFFFu;
    self->field_01c0 = 16;

    CGuiEdit_InitializeState(self, nullptr);

    self->m_pText = 0;
    if (parent != 0) {
        void* parentObject = reinterpret_cast<void*>(parent);
        self->field_0138 = static_cast<std::uint32_t>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, parentObject, 0)[0x44 / 4])(
                    parentObject, nullptr));
    } else {
        self->field_0138 = 0;
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x007214F0, CGuiEdit_Construct, "auto");
