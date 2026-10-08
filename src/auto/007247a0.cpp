#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearItems, 0x00716320,
         (CGuiListBox* self, void* edx));
CSF_FUNC(int*, __cdecl, ResetWideStringRecordRange, 0x007241E0,
         (int* begin, int* end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetSliderLinkedValue, 0x007143D0,
         (CGuiListBox* self, void* edx, int valueBits));

static int __fastcall CGuiListBox_Reset(
    CGuiListBox* self, void* /*edx*/)
{
    CGuiListBoxBase_ClearItems(self, nullptr);

    void* rowText = reinterpret_cast<void*>(self->m_pRowText);
    reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, rowText, 0)[0x3C / 4])(
            rowText, nullptr, CSF_STR(0x0082E65C, ""));

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
    self->field_022c = 1.0f;

    return CGuiListBoxBase_SetSliderLinkedValue(self, nullptr, 0x3F800000);
}

HOOK(0x007247A0, CGuiListBox_Reset, "auto");
