#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiTextureListBox_AddRows, 0x007178D0,
         (CGuiTextureListBox* self, void* edx, int count));

CSF_FUNC(int, __fastcall, CGuiListBoxBase_UpdateSliderFromVisibleRowCount,
         0x00714310, (CGuiListBoxBase* self, void* edx));

static int __fastcall CGuiTextureListBox_ExtendToRowIndex(
    CGuiTextureListBox* self, void* /*edx*/, int rowIndex)
{
    if (rowIndex == -1)
    {
        CGuiTextureListBox_AddRows(self, nullptr, 1);
    }
    else
    {
        const int currentCount =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

        if (rowIndex < currentCount)
            return currentCount;

        const int refreshedCount =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

        const int additionalRows = static_cast<int32_t>(
            static_cast<uint32_t>(rowIndex) -
            static_cast<uint32_t>(refreshedCount) + 1u);

        CGuiTextureListBox_AddRows(self, nullptr, additionalRows);
    }

    self->m_bLayoutDirty = 1;
    return CGuiListBoxBase_UpdateSliderFromVisibleRowCount(self, nullptr);
}

HOOK(0x00717970, CGuiTextureListBox_ExtendToRowIndex, "auto");
