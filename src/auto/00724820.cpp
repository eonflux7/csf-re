#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiTextureListBox_SetScrollPosition, 0x007140B0,
         (CGuiListBox* self, void* edx, int position));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660,
         (CGuiListBox* self, void* edx));
CSF_FUNC(int*, __cdecl, ResetWideStringRecordRange, 0x007241E0,
         (int* begin, int* end));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int*, __fastcall, CGuiTextureListBox_ClearBaseStorage, 0x007242D0,
         (CGuiListBox* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiStatic_GetField0134, 0x0072FF80,
         (CGuiListBox* self, void* edx));

static int __fastcall CGuiListBox_ClearRowsAndResetLinkedControl(
    CGuiListBox* self, void* /*edx*/)
{
    self->m_bLayoutDirty = 1;
    CGuiTextureListBox_SetScrollPosition(self, nullptr, 0);
    CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

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

    CGuiTextureListBox_ClearBaseStorage(self, nullptr);

    const int result = CGuiStatic_GetField0134(self, nullptr);
    if (!result) {
        return result;
    }

    void* linkedControl =
        reinterpret_cast<void*>(CGuiStatic_GetField0134(self, nullptr));
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, linkedControl, 0)[0x26C / 4])(
            linkedControl, nullptr, 0, 0);
}

HOOK(0x00724820, CGuiListBox_ClearRowsAndResetLinkedControl, "auto");
