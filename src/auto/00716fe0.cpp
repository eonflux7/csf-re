#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ComputeScrollRange, 0x00714100, (CGuiListBoxBase* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0, (CGuiStatic* self, void* , int* target));
CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280, (CGuiControl* self, void* , int eventId));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));

static int __fastcall CGuiListBoxBase_HandleEvent(
    CGuiListBoxBase* self, void* /*edx*/, int eventId)
{
    switch (eventId)
    {
    case 2:
    case 3:
        if (self->m_pSlider)
            reinterpret_cast<CGuiSlider*>(self->m_pSlider)->vSendEvent(eventId);

        CGuiStatic_CopyPropertiesToObject(
            reinterpret_cast<CGuiStatic*>(self), nullptr,
            reinterpret_cast<int*>(self->field_0138));
        break;

    case 6:
        if (self->m_pSlider)
            reinterpret_cast<CGuiSlider*>(self->m_pSlider)->vSendEvent(eventId);

        if (self->field_0138)
        {
            void** targetVtable =
                CSF_AT(void**, reinterpret_cast<void*>(self->field_0138), 0);
            alignas(8) uint8_t colorBytes[0x18];
            float* color =
                self->vGetEffectiveColor(reinterpret_cast<float*>(colorBytes));
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                targetVtable[0x68 / 4])(
                    reinterpret_cast<void*>(self->field_0138), nullptr, color);
        }
        break;

    case 13:
    {
        void* handlers =
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0);
        void* handler = CSF_AT(void*, handlers, 0);
        if (handler)
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x24 / 4])(handler, nullptr, 0);
        break;
    }

    case 15:
    {
        void* parent = reinterpret_cast<void*>(self->vGetParent());
        void* related =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, parent, 0)[0x104 / 4])(parent, nullptr);
        if (related &&
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, related, 0)[0x188 / 4])(related, nullptr))
            break;

        if ((reinterpret_cast<uintptr_t>(self->vSlot140()) & 1) == 0)
            break;

        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<CGuiSlider*>(self), nullptr);

        if ((reinterpret_cast<uintptr_t>(self->vSlot140()) & 0x10) != 0)
            break;

        void* handlers =
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0);
        void* handler = CSF_AT(void*, handlers, 4);
        if (handler)
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x24 / 4])(handler, nullptr, 0);
        break;
    }

    case 18:
    {
        if (!reinterpret_cast<int(__fastcall*)(CGuiListBoxBase*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr))
            break;

        if (!self->vIsSelected())
            CGuiSlider_ForwardSelfToParent(
                reinterpret_cast<CGuiSlider*>(self), nullptr);

        char* subObject = self->vGetSubObject0D8();
        int relativeY = static_cast<int>(
            CSF_AT(uint32_t, subObject, 4) -
            self->field_0178 - self->field_0174);
        if (relativeY < 0)
            break;

        int previousValue = -1;
        if (CGuiListBox_GetItemCount_714860(
                reinterpret_cast<CGuiListBox*>(self), nullptr) > 0)
            previousValue = GetStoredValueAtIndex(self, nullptr, 0);

        CGuiListBoxBase_ClearStorageAndSendEvent(self, nullptr);

        int scrollPosition = 0;
        if (self->m_pSlider)
        {
            void* slider = reinterpret_cast<void*>(self->m_pSlider);
            scrollPosition =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, slider, 0)[0x270 / 4])(slider, nullptr);
        }

        int clickedRow = relativeY /
            static_cast<int>(self->m_rowHeight + 2 * self->m_lineSpacing);
        int scrollRange = CGuiListBoxBase_ComputeScrollRange(self, nullptr);
        int rowStride =
            static_cast<int>(self->m_rowHeight + 2 * self->m_lineSpacing);
        int visibleRows = (scrollRange - scrollRange % rowStride) / rowStride;

        int selectedIndex = clickedRow + scrollPosition;
        if (clickedRow >= visibleRows)
            selectedIndex = visibleRows + scrollPosition - 1;

        if (clickedRow >=
            reinterpret_cast<int(__fastcall*)(CGuiListBoxBase*, void*)>(
                CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr))
        {
            selectedIndex =
                reinterpret_cast<int(__fastcall*)(CGuiListBoxBase*, void*)>(
                    CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr) - 1;
        }

        CGuiListBoxBase_AddSelectedItem(self, nullptr, selectedIndex);

        if (previousValue != GetStoredValueAtIndex(self, nullptr, 0))
        {
            void* handlers =
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* handler = CSF_AT(void*, handlers, 4);
            if (handler)
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, handler, 0)[0x24 / 4])(handler, nullptr, 0);
        }
        break;
    }

    case 20:
        self->vDispatchEvent13ForNonemptyList();
        break;

    default:
        break;
    }

    return CGuiControl_SendEvent(self, nullptr, eventId);
}

HOOK(0x00716FE0, CGuiListBoxBase_HandleEvent, "auto");
