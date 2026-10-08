#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_GuiListBoxVtable, 0x00865070);

CSF_FUNC(int*, __cdecl, ResetWideStringRecordRange, 0x007241E0,
         (int* begin, int* end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, DestroyListBoxBase, 0x00716110,
         (CGuiListBox* self, void* edx));

static int __fastcall CGuiListBox_Destroy(
    CGuiListBox* self, void* /*edx*/)
{
    void* rowText = reinterpret_cast<void*>(self->m_pRowText);
    CSF_AT(void**, self, 0) =
        reinterpret_cast<void**>(&g_GuiListBoxVtable);

    if (rowText) {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, rowText, 0)[0])(rowText, nullptr, 1);
        self->m_pRowText = 0;
    }

    int* begin = reinterpret_cast<int*>(self->field_0220);
    if (begin) {
        ResetWideStringRecordRange(
            begin, reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x224)));

        if (self->field_0220) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0220));
        }
    }

    self->field_0220 = 0;
    CSF_AT(uint32_t, self, 0x224) = 0;
    CSF_AT(uint32_t, self, 0x228) = 0;

    return DestroyListBoxBase(self, nullptr);
}

HOOK(0x00724720, CGuiListBox_Destroy, "auto");
