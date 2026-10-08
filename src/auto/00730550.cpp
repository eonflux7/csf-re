#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* target));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, int* target));
CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int event));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiSwitch_ToggleStateAndUpdateColor, 0x007303E0,
         (CGuiSwitch* self, void* edx));

static void CGuiSwitch_RefreshChildState(
    CGuiSwitch* self, std::size_t childOffset, void* stateBuffer)
{
    void* child = CSF_AT(void*, self, childOffset);
    if (child)
    {
        void** childVtable = CSF_AT(void**, child, 0);
        int state = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, self, 0)[0x228 / 4])(
                self, nullptr, stateBuffer);

        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            childVtable[0x68 / 4])(
                CSF_AT(void*, self, childOffset), nullptr, state);
    }
}

static int __fastcall CGuiSwitch_HandleEvent(
    CGuiSwitch* self, void* /*edx*/, int event)
{
    switch (event)
    {
    case 2:
    case 3:
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, CSF_AT(int*, self, 0x138));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, CSF_AT(int*, self, 0x134));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, CSF_AT(int*, self, 0x13C));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, CSF_AT(int*, self, 0x140));
        CGuiStatic_CopyStateToTarget(
            self, nullptr, CSF_AT(int*, self, 0x148));
        break;

    case 6:
    {
        alignas(4) std::uint8_t firstState[16];
        alignas(4) std::uint8_t secondState[16];
        alignas(4) std::uint8_t thirdState[16];
        alignas(4) std::uint8_t fourthState[16];

        CGuiSwitch_RefreshChildState(self, 0x138, firstState);
        CGuiSwitch_RefreshChildState(self, 0x134, secondState);
        CGuiSwitch_RefreshChildState(self, 0x13C, thirdState);
        CGuiSwitch_RefreshChildState(self, 0x140, fourthState);
        break;
    }

    case 18:
    case 20:
        if ((reinterpret_cast<int(__fastcall*)(void*, void*)>(
                 CSF_AT(void**, self, 0)[0x140 / 4])(
                     self, nullptr) & 1) != 0)
        {
            void* container = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* recipient = CSF_AT(void*, container, 4);
            if (recipient)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, recipient, 0)[0x24 / 4])(
                        recipient, nullptr, 0);
            }
            CGuiSlider_ForwardSelfToParent(self, nullptr);
        }
        break;

    case 19:
        if ((reinterpret_cast<int(__fastcall*)(void*, void*)>(
                 CSF_AT(void**, self, 0)[0x140 / 4])(
                     self, nullptr) & 1) != 0
            && reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
                   CSF_AT(void**, self, 0)[0xBC / 4])(
                       self, nullptr) != 0)
        {
            void* container = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* recipient = CSF_AT(void*, container, 0);
            if (recipient)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, recipient, 0)[0x24 / 4])(
                        recipient, nullptr, 0);
            }
            CGuiSwitch_ToggleStateAndUpdateColor(self, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x138 / 4])(
                    self, nullptr, 13);
        }
        break;

    default:
        break;
    }

    return CGuiControl_SendEvent(self, nullptr, event);
}

HOOK(0x00730550, CGuiSwitch_HandleEvent, "auto");
