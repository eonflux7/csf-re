#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiTextureListBox_SetScrollPosition, 0x007140B0,
         (CGuiTextureListBox* self, void* edx, int position));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660,
         (CGuiTextureListBox* self, void* edx));
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40,
         (int* begin, int* end, void* container, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));
CSF_FUNC(int*, __fastcall, CGuiTextureListBox_ClearBaseStorage, 0x007242D0,
         (CGuiTextureListBox* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiStatic_GetField0134, 0x0072FF80,
         (CGuiTextureListBox* self, void* edx));

// Clears the texture list box's row-record storage (destroying the record
// range and freeing the buffer), resets the flags, refreshes its rows and
// then resets the associated linked control through its vtable.
static int __fastcall CGuiTextureListBox_ClearRowsAndResetLinkedControl(
    CGuiTextureListBox* self, void* /*edx*/)
{
    self->m_bLayoutDirty = 1;
    CGuiTextureListBox_SetScrollPosition(self, nullptr, 0);
    CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

    const uint32_t recordData = self->field_01fc;
    void* const recordContainer = reinterpret_cast<uint8_t*>(self) + 0x1F8;

    if (recordData != 0) {
        ResetRecordRange(
            reinterpret_cast<int*>(recordData),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x200)),
            recordContainer,
            self);

        if (self->field_01fc != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_01fc));
        }
    }

    self->field_01fc = 0;
    CSF_AT(uint32_t, self, 0x200) = 0;
    CSF_AT(uint32_t, self, 0x204) = 0;

    CGuiTextureListBox_ClearBaseStorage(self, nullptr);

    const int result = CGuiStatic_GetField0134(self, nullptr);
    if (result == 0) {
        return result;
    }

    void* const linkedControl =
        reinterpret_cast<void*>(CGuiStatic_GetField0134(self, nullptr));
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, linkedControl, 0)[0x26C / 4])(
            linkedControl, nullptr, 0, 0);
}

HOOK(0x007316E0, CGuiTextureListBox_ClearRowsAndResetLinkedControl, "auto");
