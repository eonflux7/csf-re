#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsSelectableAndNotType10, 0x00718700,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, ClearSelectionAndSendEvent, 0x00715660,
         (CGuiListBoxBase* self, void* edx));
// Capture EAX even though this callee is nominally void.
CSF_FUNC(int, __fastcall, SelectRow, 0x00716A60,
         (CGuiListBoxBase* self, void* edx, int row));
CSF_GLOBAL(char*, g_NamedEntryName, 0x008A85E4);

static int32_t SignedWord(uint32_t value)
{
    return static_cast<int32_t>(value);
}

static int32_t CGuiListBoxBase_GetSelectionCount(CGuiListBoxBase* self)
{
    const uint32_t begin = self->field_01ac;
    return begin ? SignedWord(self->field_01b0 - begin) >> 2 : 0;
}

static int __fastcall CGuiListBoxBase_UpdateSelectionDisplay(
    CGuiListBoxBase* self, void* /*edx*/)
{
    if (self->field_01dd)
    {
        const int32_t count = CGuiListBoxBase_GetSelectionCount(self);
        // SETNE replaces only AL; the remaining bits of the count survive.
        const uint32_t enabled =
            (static_cast<uint32_t>(count) & 0xFFFFFF00u) |
            static_cast<uint32_t>(count != 0);
        char* name = g_NamedEntryName;
        ((void (__fastcall*)(void*, void*, int, char*))
            CSF_AT(void**, self, 0)[0x20C / 4])(
                self, nullptr, SignedWord(enabled), name);
    }

    const uint32_t columnsBegin = self->m_columnsBegin;
    if (!columnsBegin)
        return 0;

    const int32_t columnBytes =
        SignedWord(self->m_columnsEnd - columnsBegin);
    const int64_t product = int64_t(columnBytes) * int64_t(0x2AAAAAAB);
    const int32_t high = static_cast<int32_t>(product >> 32);
    const int32_t shiftedHigh = high >> 1;
    const int32_t columnCount =
        shiftedHigh + static_cast<int32_t>(
            static_cast<uint32_t>(shiftedHigh) >> 31);
    if (!columnCount)
        return SignedWord(static_cast<uint32_t>(product));

    int result = IsSelectableAndNotType10(self, nullptr);
    if (!(static_cast<uint32_t>(result) & 0xFFu))
        return result;

    int32_t origin[2];
    ((void (__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, origin);

    origin[1] = SignedWord(
        static_cast<uint32_t>(origin[1]) +
        self->field_0178 + self->field_0174);

    void* dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    const int32_t availableHeight = SignedWord(
        CSF_AT(uint32_t, dimensions, 4) -
        self->field_0178 - self->field_0174);
    const int32_t rowPitch =
        SignedWord(self->field_01b8 + 2u * self->m_lineSpacing);
    const int32_t remainder = availableHeight % rowPitch;
    const uint8_t drawSelection = self->field_01dd;
    const int32_t wholeHeight = SignedWord(
        static_cast<uint32_t>(availableHeight) -
        static_cast<uint32_t>(remainder));
    const int32_t endRow = SignedWord(
        static_cast<uint32_t>(wholeHeight / rowPitch) + self->field_0168);
    result = endRow;

    if (drawSelection)
    {
        const uint32_t count = static_cast<uint32_t>(
            CGuiListBoxBase_GetSelectionCount(self));

        for (uint32_t index = 0; index < count; ++index)
        {
            int32_t* selectedRow = reinterpret_cast<int32_t*>(
                self->field_01ac + index * 4u);
            const int32_t firstVisible = SignedWord(self->field_0168);
            if (*selectedRow < firstVisible || *selectedRow >= endRow)
                continue;

            const uint32_t rowOffset =
                static_cast<uint32_t>(*selectedRow) -
                static_cast<uint32_t>(firstVisible);
            const uint32_t pitch =
                self->field_01b8 + 2u * self->m_lineSpacing;
            const uint32_t topOffset =
                rowOffset * pitch + self->field_01d0;
            const uint32_t left =
                static_cast<uint32_t>(origin[0]) + self->field_01cc;
            const uint32_t right = left + self->field_01d4;
            const uint32_t height = self->field_01d8;
            const uint32_t top =
                static_cast<uint32_t>(origin[1]) + topOffset;

            uint32_t rectangle[4] = {left, top, right, top + height};
            char* name = g_NamedEntryName;
            ((void (__fastcall*)(void*, void*, uint32_t*, char*))
                CSF_AT(void**, self, 0)[0x208 / 4])(
                    self, nullptr, rectangle, name);
        }
    }

    int32_t* selectedRows =
        reinterpret_cast<int32_t*>(self->field_01ac);
    if (!selectedRows)
        return result;

    const uint32_t selectionBegin =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(selectedRows));
    if ((SignedWord(self->field_01b0 - selectionBegin) >> 2) <= 0)
        return result;
    if ((SignedWord(self->field_01b0 - selectionBegin) >> 2) == 0)
        return result;

    const int32_t selectedRow = *selectedRows;
    if (selectedRow == -1)
        return result;

    if (selectedRow < SignedWord(self->field_0168))
    {
        ClearSelectionAndSendEvent(self, nullptr);
        return SelectRow(self, nullptr, SignedWord(self->field_0168));
    }

    const int32_t lastVisible =
        SignedWord(static_cast<uint32_t>(endRow) - 1u);
    if (selectedRow > lastVisible)
    {
        ClearSelectionAndSendEvent(self, nullptr);
        return SelectRow(self, nullptr, lastVisible);
    }

    return result;
}

HOOK(0x007156B0, CGuiListBoxBase_UpdateSelectionDisplay, "auto");
