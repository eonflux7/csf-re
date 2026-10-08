#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(unsigned int, __fastcall, AppendSelectionIndex, 0x005570F0,
         (void* indices, void* edx, int* index));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_UpdateSelectionDisplay, 0x007156B0,
         (CGuiListBoxBase* self, void* edx));

static void __fastcall CGuiListBoxBase_AddSelectedItem(
    CGuiListBoxBase* self, void* /*edx*/, int index)
{
    const int requestedIndex = index;
    if (requestedIndex < 0)
        return;

    const int itemCount =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
    if (requestedIndex >= itemCount)
        return;

    const std::uint32_t begin = self->field_01ac;
    std::uint32_t selectionCount = 0;
    if (begin != 0)
    {
        const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x1B0);
        selectionCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(end - begin) >> 2);
    }

    std::uint32_t position = 0;
    if (selectionCount != 0)
    {
        const int* entry = reinterpret_cast<const int*>(self->field_01ac);
        do
        {
            if (*entry == requestedIndex)
                break;
            ++position;
            ++entry;
        } while (position < selectionCount);
    }

    if (position != selectionCount)
        return;

    AppendSelectionIndex(
        reinterpret_cast<std::uint8_t*>(self) + 0x1A8, nullptr, &index);

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 23);

    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider != nullptr)
    {
        self->field_0168 =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, slider, 0)[0x270 / 4])(slider, nullptr);
    }
    else
    {
        self->field_0168 = self->field_0168;
    }

    CGuiListBoxBase_UpdateSelectionDisplay(self, nullptr);
}

HOOK(0x00716A60, CGuiListBoxBase_AddSelectedItem, "auto");
