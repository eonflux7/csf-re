#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_HandleSelectionInput_718510, 0x00718510,
         (CGuiControl* self, void* edx, int direction, int input));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));

// Original switch jump table; its dispatch is reproduced below.
CSF_GLOBAL(const std::uint32_t, g_DirectionDispatchTable, 0x0072B7F0);

static char __fastcall CGuiPanel_HandleDirectionalInput(
    CGuiPanel* self, void* /*edx*/, int direction, int input)
{
    char result =
        ((char(__fastcall*)(CGuiControl*, void*))self->vtbl[0x6C / 4])(
            self, nullptr);
    if (result == 0)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (result == 0)
        return result;

    const std::uint32_t initialBegin = self->m_childrenBegin;
    const std::uint32_t initialCount =
        initialBegin != 0
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      self->m_childrenEnd - initialBegin) >> 2)
            : 0;

    for (std::uint32_t index = 0; index < initialCount; ++index)
    {
        auto* child = reinterpret_cast<CGuiControl*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->m_childrenBegin),
                   index * 4));

        ((void(__fastcall*)(CGuiControl*, void*, int, int))
             child->vtbl[0x38 / 4])(child, nullptr, direction, input);
    }

    result =
        CGuiControl_HandleSelectionInput_718510(self, nullptr, direction, input);

    auto* currentControl =
        reinterpret_cast<CGuiControl*>(self->field_014c);
    if (currentControl == nullptr)
        return result;

    CGuiControl* focusCandidate = nullptr;
    std::uint32_t attempts = 0;

    for (;;)
    {
        const char* navigationId = nullptr;

        if (static_cast<std::uint32_t>(direction) <= 3 &&
            currentControl != nullptr)
        {
            std::uint32_t slot;
            switch (direction)
            {
            case 0: slot = 0x90 / 4; break;
            case 1: slot = 0x94 / 4; break;
            case 2: slot = 0x88 / 4; break;
            default: slot = 0x8C / 4; break;
            }

            navigationId =
                ((const char*(__fastcall*)(CGuiControl*, void*))
                     currentControl->vtbl[slot])(currentControl, nullptr);

            if (navigationId != nullptr && *navigationId != 0)
            {
                currentControl =
                    ((CGuiControl*(__fastcall*)(
                         CGuiPanel*, void*, const char*))
                         self->vtbl[0xFC / 4])(
                        self, nullptr, navigationId);

                if (currentControl != nullptr &&
                    ((char(__fastcall*)(CGuiControl*, void*))
                         currentControl->vtbl[0x6C / 4])(
                        currentControl, nullptr) != 0 &&
                    static_cast<std::uint8_t>(
                        CGuiSlider_IsSelectableAndNotType10(
                            currentControl, nullptr)) != 0)
                {
                    focusCandidate = currentControl;
                }
            }
        }

        ++attempts;
        const std::uint32_t childrenBegin = self->m_childrenBegin;

        if (childrenBegin != 0 &&
            attempts < static_cast<std::uint32_t>(
                           static_cast<std::int32_t>(
                               self->m_childrenEnd - childrenBegin) >> 2))
        {
            if (focusCandidate == nullptr)
            {
                if (navigationId == nullptr || *navigationId == 0)
                    return 0;
                continue;
            }
        }
        else if (focusCandidate == nullptr)
        {
            return 0;
        }

        ((void(__fastcall*)(CGuiPanel*, void*, CGuiControl*))
             self->vtbl[0x158 / 4])(self, nullptr, focusCandidate);

        void* builderSubobject =
            reinterpret_cast<void*>(self->field_0010);
        void* builder = CSF_AT(void*, builderSubobject, 0);
        void* inputHandler = CSF_AT(void*, builder, 4);

        if (inputHandler != nullptr)
        {
            return static_cast<char>(
                ((int(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, inputHandler, 0)[0x24 / 4])(
                    inputHandler, nullptr, 0));
        }

        return static_cast<char>(
            reinterpret_cast<std::uintptr_t>(builderSubobject));
    }
}

HOOK(0x0072B660, CGuiPanel_HandleDirectionalInput, "auto");
