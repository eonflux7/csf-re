#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int (__fastcall* const CGuiListBoxBase_ClearStorageAndSendEvent)(
    CGuiListBoxBase*, void*) =
    reinterpret_cast<int (__fastcall*)(CGuiListBoxBase*, void*)>(CSF_ADDR_FN(0x00715660));

static void (__fastcall* const CGuiListBoxBase_SelectItem)(
    CGuiListBoxBase*, void*, int) =
    reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*, int)>(CSF_ADDR_FN(0x00716A60));

static int __fastcall CGuiListBoxBase_SelectNextItem(
    CGuiListBoxBase* self, void* /*edx*/)
{
    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
    if (result <= 0)
        return result;

    int previousSelection = -1;
    int selectedItem = -1;
    uint32_t selectionBegin = self->field_01ac;

    if (!selectionBegin ||
        (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) <= 0)
    {
        CGuiListBoxBase_SelectItem(self, nullptr, 0);
    }
    else
    {
        if (selectionBegin &&
            (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) != 0)
        {
            selectedItem = CSF_AT(int32_t,
                reinterpret_cast<void*>(self->field_01ac), 0);
            previousSelection = selectedItem;
        }

        const int itemCount = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

        if (selectedItem < static_cast<int32_t>(
                static_cast<uint32_t>(itemCount) - 1u))
        {
            CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

            void* size = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

            const uint32_t height = CSF_AT(uint32_t, size, 4);
            const uint32_t bottomInset = self->field_0178;
            const uint32_t topInset = self->field_0174;
            const uint32_t itemHeight = self->field_01b8;
            const uint32_t lineSpacing = self->m_lineSpacing;

            const int32_t availableHeight =
                static_cast<int32_t>(height - bottomInset - topInset);
            const int32_t rowHeight =
                static_cast<int32_t>(itemHeight + lineSpacing * 2u);
            const int32_t remainder = availableHeight % rowHeight;
            const uint32_t firstVisibleItem = self->field_0168;

            const int32_t nextItem =
                static_cast<int32_t>(static_cast<uint32_t>(selectedItem) + 1u);
            const int32_t roundedHeight = static_cast<int32_t>(
                static_cast<uint32_t>(availableHeight) -
                static_cast<uint32_t>(remainder));
            const int32_t visibleRows = roundedHeight / rowHeight;
            const int32_t lastVisibleItem = static_cast<int32_t>(
                static_cast<uint32_t>(visibleRows) + firstVisibleItem - 1u);

            if (nextItem > lastVisibleItem)
            {
                void* slider = reinterpret_cast<void*>(self->m_pSlider);
                if (slider)
                {
                    void** sliderVtable = CSF_AT(void**, slider, 0);
                    const int sliderPosition =
                        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                            sliderVtable[0x270 / 4])(slider, nullptr, 0);

                    slider = reinterpret_cast<void*>(self->m_pSlider);
                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                        sliderVtable[0x26C / 4])(
                            slider, nullptr,
                            static_cast<int32_t>(
                                static_cast<uint32_t>(sliderPosition) + 1u));
                }
                ++self->field_0168;
            }

            CGuiListBoxBase_SelectItem(self, nullptr, nextItem);
        }
        else if (self->m_bCycle)
        {
            CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

            void* slider = reinterpret_cast<void*>(self->m_pSlider);
            if (slider)
            {
                // This path supplies two stack arguments, unlike advancement.
                reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
                    CSF_AT(void**, slider, 0)[0x26C / 4])(
                        slider, nullptr, 0, 0);
            }

            self->field_0168 = 0;
            CGuiListBoxBase_SelectItem(self, nullptr, 0);
        }
    }

    selectionBegin = self->field_01ac;
    if (selectionBegin &&
        (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) != 0)
    {
        result = CSF_AT(int32_t,
            reinterpret_cast<void*>(self->field_01ac), 0);
    }
    else
    {
        result = -1;
    }

    if (previousSelection != result)
    {
        void* ownerLink = CSF_AT(void*, static_cast<void*>(self), 0x10);
        void* ownerRecord = CSF_AT(void*, ownerLink, 0);
        void* listener = CSF_AT(void*, ownerRecord, 4);
        if (listener)
        {
            return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, listener, 0)[0x24 / 4])(
                    listener, nullptr, 0);
        }
    }

    return result;
}

HOOK(0x00716D00, CGuiListBoxBase_SelectNextItem, "auto");
