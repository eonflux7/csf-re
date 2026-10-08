#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearItems, 0x00716320,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (int* begin, int* end, ...));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetSliderLinkedValue, 0x007143D0,
         (CGuiListBoxBase* self, void* edx, int valueBits));

static int __fastcall CGuiTextureListBox_ClearTextureRows(
    CGuiTextureListBox* self, void* /*edx*/)
{
    CGuiListBoxBase_ClearItems(self, nullptr);

    int* begin = reinterpret_cast<int*>(self->field_01fc);
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 0x1F8;

    if (begin != nullptr) {
        ResetRecordRange(
            begin, reinterpret_cast<int*>(self->m_textureRowsEnd),
            storage, self);

        begin = reinterpret_cast<int*>(self->field_01fc);
        if (begin != nullptr)
            Mem_FreeDebug(begin);
    }

    self->field_01fc = 0;
    self->m_textureRowsEnd = 0;
    CSF_AT(std::uint32_t, self, 0x204) = 0;
    self->field_0208 = 0x3F800000u;
    return CGuiListBoxBase_SetSliderLinkedValue(self, nullptr, 0x3F800000);
}

HOOK(0x00731320, CGuiTextureListBox_ClearTextureRows, "auto");
