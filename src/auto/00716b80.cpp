#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ComputeScrollRange, 0x00714100,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60,
         (CGuiListBoxBase* self, void* edx, int index));

static int __fastcall CGuiListBoxBase_SelectPreviousItem(
    CGuiListBoxBase* self, void* /*edx*/)
{
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
    if (result <= 0)
        return result;

    int previousSelection = -1;
    uint32_t selectionBegin = self->field_01ac;

    if (selectionBegin != 0 &&
        (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) > 0)
    {
        if (selectionBegin != 0 &&
            (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) != 0)
        {
            previousSelection =
                *reinterpret_cast<int32_t*>(self->field_01ac);
        }

        if (previousSelection > 0)
        {
            CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

            const int precedingIndex = previousSelection - 1;
            if (precedingIndex < static_cast<int32_t>(self->field_0168))
            {
                void* slider = reinterpret_cast<void*>(self->m_pSlider);
                if (slider != nullptr)
                {
                    void** sliderVtable = CSF_AT(void**, slider, 0);
                    // The original stages the setter's second argument (0)
                    // before calling this zero-argument getter.
                    const int position =
                        reinterpret_cast<int(__fastcall*)(void*, void*)>(
                            sliderVtable[0x270 / 4])(slider, nullptr);

                    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                        sliderVtable[0x26c / 4])(
                            reinterpret_cast<void*>(self->m_pSlider), nullptr,
                            static_cast<int32_t>(
                                static_cast<uint32_t>(position) - 1u),
                            0);
                }
                self->field_0168 -= 1u;
            }

            CGuiListBoxBase_AddSelectedItem(self, nullptr, precedingIndex);
        }
        else if (self->m_bCycle != 0)
        {
            CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);
            const int range =
                CGuiListBoxBase_ComputeScrollRange(self, nullptr);
            const int32_t rowPitch = static_cast<int32_t>(
                self->field_01b8 + 2u * self->m_lineSpacing);

            const int remainder = range % rowPitch;
            const int wholeRange = static_cast<int32_t>(
                static_cast<uint32_t>(range) -
                static_cast<uint32_t>(remainder));
            const int visibleRows = wholeRange / rowPitch;

            const int itemCount =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);

            int scrollPosition;
            if (visibleRows >= itemCount)
            {
                scrollPosition = 0;
            }
            else
            {
                const int currentCount =
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
                scrollPosition = static_cast<int32_t>(
                    static_cast<uint32_t>(currentCount) -
                    static_cast<uint32_t>(visibleRows));
            }

            void* slider = reinterpret_cast<void*>(self->m_pSlider);
            self->field_0168 = static_cast<uint32_t>(scrollPosition);
            if (slider != nullptr)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                    CSF_AT(void**, slider, 0)[0x26c / 4])(
                        slider, nullptr, scrollPosition, 0);
            }

            const int currentCount =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
            CGuiListBoxBase_AddSelectedItem(
                self, nullptr,
                static_cast<int32_t>(
                    static_cast<uint32_t>(currentCount) - 1u));
        }
    }
    else
    {
        CGuiListBoxBase_AddSelectedItem(self, nullptr, 0);
    }

    selectionBegin = self->field_01ac;
    if (selectionBegin != 0 &&
        (static_cast<int32_t>(self->field_01b0 - selectionBegin) >> 2) != 0)
    {
        result = *reinterpret_cast<int32_t*>(self->field_01ac);
    }
    else
    {
        result = -1;
    }

    if (previousSelection != result)
    {
        void* controlLink = reinterpret_cast<void*>(self->field_0010);
        void* linkedObject = CSF_AT(void*, controlLink, 0);
        void* listener = CSF_AT(void*, linkedObject, 4);
        if (listener != nullptr)
        {
            return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, listener, 0)[0x24 / 4])(
                    listener, nullptr, 0);
        }
    }

    return result;
}

HOOK(0x00716B80, CGuiListBoxBase_SelectPreviousItem, "auto");
