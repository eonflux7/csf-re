#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(CGuiListBoxBase*, __fastcall, CGuiListBoxBase_Construct, 0x007166C0,
         (CGuiListBoxBase* self, void* edx, int parentAddress, int argument));


static CGuiListBox* __fastcall CGuiListBox_Construct(
    CGuiListBox* self, void* /*edx*/, int parentAddress, int argument)
{
    CGuiListBoxBase_Construct(
        reinterpret_cast<CGuiListBoxBase*>(self), nullptr, parentAddress, argument);

    CSF_AT(void**, self, 0x000) = CSF_VTBL(0x00865070);

    self->field_01f8 = 0;
    self->field_01fc = 0;
    self->field_0200 = 0;
    self->field_0204 = 0x3F800000u;

    self->m_selTextColor[0] = 0.0f;
    self->m_selTextColor[1] = 0.0f;
    self->m_selTextColor[2] = 0.0f;
    self->m_selTextColor[3] = 1.0f;

    self->field_0220 = 0;
    self->field_0224 = 0;
    self->field_0228 = 0;
    self->field_022c = 1.0f;

    self->field_01f8 = 0x437F0000u;
    self->field_01fc = 0x437F0000u;
    self->field_0200 = 0x437F0000u;
    self->field_0204 = 0x42FE0000u;

    self->m_selTextColor[0] = 255.0f;
    self->m_selTextColor[1] = 255.0f;
    self->m_selTextColor[2] = 255.0f;
    self->m_selTextColor[3] = 255.0f;

    void* parent = reinterpret_cast<void*>(parentAddress);
    void* rowText =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, parent, 0)[0x58 / 4])(parent, nullptr);

    self->m_pRowText = reinterpret_cast<std::uint32_t>(rowText);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, rowText, 0)[0x68 / 4])(rowText, nullptr, 1);

    void* reloadedRowText = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, reloadedRowText, 0)[0x6C / 4])(
            reloadedRowText, nullptr, 1);

    return self;
}

HOOK(0x00724A30, CGuiListBox_Construct, "auto");
