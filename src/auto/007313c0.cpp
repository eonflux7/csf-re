#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(CGuiListBoxBase*, __fastcall, CGuiListBoxBase_Construct,
         0x007166C0, (CGuiListBoxBase* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetLineSpacing,
         0x00714810, (CGuiListBoxBase* self, void* edx, int spacing));

CSF_GLOBAL(void*, g_CGuiTextureListBoxVtable, 0x00866DB8);

static CGuiTextureListBox* __fastcall CGuiTextureListBox_Construct(
    CGuiTextureListBox* self, void* /*edx*/, int a2, int a3)
{
    CGuiListBoxBase_Construct(self, nullptr, a2, a3);

    CSF_AT(void**, self, 0) = &g_CGuiTextureListBoxVtable;
    self->field_01fc = 0;
    self->m_textureRowsEnd = 0;
    self->field_0204 = 0;

    CGuiListBoxBase_SetLineSpacing(self, nullptr, 0);

    self->field_0140 = 0;
    self->field_0208 = 0x3F800000;
    return self;
}

HOOK(0x007313C0, CGuiTextureListBox_Construct, "auto");
