#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto CGuiControl_Construct =
    reinterpret_cast<int (__fastcall*)(C_00830438*, void*, void*, int)>(
        CSF_ADDR_FN(0x0071A010));

static const auto CGuiControl_GetRelatedObject =
    reinterpret_cast<void* (__fastcall*)(CGuiListBoxBase*, void*)>(
        CSF_ADDR_FN(0x00686C10));

CSF_GLOBAL(std::uint32_t, g_GuiListBoxBaseVtable, 0x00863FF8);

static CGuiListBoxBase* __fastcall CGuiListBoxBase_Construct(
    CGuiListBoxBase* self, void* /*edx*/, void* owner, int creationFlags)
{
    CGuiControl_Construct(
        reinterpret_cast<C_00830438*>(self), nullptr, owner, creationFlags);

    CSF_AT(void**, self, 0x00) =
        reinterpret_cast<void**>(&g_GuiListBoxBaseVtable);

    self->m_itemColor[0] = 0.0f;
    self->m_itemColor[1] = 0.0f;
    self->m_itemColor[2] = 0.0f;
    self->m_itemColor[3] = 1.0f;
    self->m_noFocusColor[0] = 0.0f;
    self->m_noFocusColor[1] = 0.0f;
    self->m_noFocusColor[2] = 0.0f;
    self->m_noFocusColor[3] = 1.0f;

    self->field_0174 = 0;
    self->field_0178 = 0;
    self->field_018c = 0;
    self->field_0190 = 0;
    self->field_0194 = 0;
    self->m_columnsBegin = 0;
    self->m_columnsEnd = 0;
    self->m_columnsCap = 0;
    self->field_01ac = 0;
    self->field_01b0 = 0;
    self->field_01b4 = 0;
    self->field_01bc = 0;
    self->field_01c0 = 0;
    self->field_01c4 = 0;
    self->field_01c8 = 0;
    self->field_01cc = 0;
    self->field_01d0 = 0;
    self->field_01d4 = 0;
    self->field_01d8 = 0;
    self->field_01e0 = 0;
    self->field_01e4 = 0;
    self->field_01e8 = 0;
    self->field_01ec = 0x3F800000;

    self->m_rowHeight = 20;
    self->field_01c8 = 20;
    self->field_01c4 = 20;

    self->m_itemColor[0] = 200.0f;
    self->m_itemColor[1] = 200.0f;
    self->m_itemColor[2] = 255.0f;
    self->m_noFocusColor[0] = 255.0f;
    self->m_noFocusColor[1] = 255.0f;
    self->m_noFocusColor[2] = 255.0f;

    self->field_0140 = 1;
    self->field_01dc = 1;
    self->m_pSlider = 0;
    self->field_0138 = 0;
    self->m_itemColor[3] = 127.0f;
    self->m_noFocusColor[3] = 127.0f;
    self->field_0168 = 0;
    self->field_0164 = 0;
    self->field_01dd = 0;
    self->m_pHeaderText = 0;
    self->field_0184 = 0;
    self->m_bLines = 0;
    self->m_bAutoSize = 0;
    self->m_bRowFill = 0;
    self->m_bLayoutDirty = 0;
    self->m_lineSpacing = 2;
    self->field_01f0 = 1.0f;
    self->m_bCycle = 0;

    self->field_017c =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0x00)[0x44 / 4])(owner, nullptr);

    void* relatedObject = CGuiControl_GetRelatedObject(self, nullptr);
    self->field_013c =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, relatedObject, 0x00)[0x48 / 4])(
                relatedObject, nullptr);

    return self;
}

HOOK(0x007166C0, CGuiListBoxBase_Construct, "auto");
