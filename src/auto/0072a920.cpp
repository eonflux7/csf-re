#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x00718C00,
         (CGuiControl* self, void* edx, int eventValue));

static char __fastcall CGuiPanel_ResetChildFocusAndAnimation(
    CGuiPanel* self, void* /*edx*/)
{
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x268 / 4])(self, nullptr);

    const std::uint32_t childrenBegin = self->field_0138;
    const std::int32_t childCount = childrenBegin
        ? (static_cast<std::int32_t>(self->field_013c - childrenBegin) >> 2)
        : 0;
    bool forwardedChild = false;

    for (std::int32_t index = childCount - 1; index >= 0; --index)
    {
        auto* child =
            reinterpret_cast<CGuiControl**>(self->field_0138)[index];
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[0x16C / 4])(child, nullptr);

        // Reload: the preceding virtual call may have changed the child list.
        child = reinterpret_cast<CGuiControl**>(self->field_0138)[index];

        std::uint32_t pointStorage[2];
        auto* relativePoint =
            ((std::uint32_t*(__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, self, 0)[0x160 / 4])(
                    self, nullptr, pointStorage);
        auto* origin =
            ((std::uint32_t*(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x148 / 4])(self, nullptr);

        std::uint32_t point[2];
        point[1] = relativePoint[1] + origin[1];
        point[0] = relativePoint[0] + origin[0];

        const std::uint8_t containsPoint =
            ((std::uint8_t(__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, child, 0)[0x154 / 4])(
                    child, nullptr, point);

        child = reinterpret_cast<CGuiControl**>(self->field_0138)[index];
        if (containsPoint)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[0x144 / 4])(child, nullptr, 1);

            if (self->field_0189)
            {
                child =
                    reinterpret_cast<CGuiControl**>(self->field_0138)[index];
                const int controlType =
                    ((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr);

                if (controlType != 10)
                {
                    child =
                        reinterpret_cast<CGuiControl**>(self->field_0138)[index];
                    CGuiSlider_ForwardSelfToParent(child, nullptr);
                    forwardedChild = true;
                }
            }
        }
        else
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[0x144 / 4])(child, nullptr, 2);
        }
    }

    CGuiControl_ResetInput(self, nullptr);
    self->field_0178 = 0.0f;
    char result = static_cast<char>(
        CGuiControl_ForwardEventToFirstEffect(self, nullptr, 0x3F800000));

    auto* initialFocus =
        reinterpret_cast<CGuiControl*>(self->m_pInitialFocus);
    self->field_017c = 1;
    self->field_0180 = 0.0f;
    self->field_0184 = 0.0f;

    if (initialFocus)
    {
        result = static_cast<char>(
            ((std::uint8_t(__fastcall*)(void*, void*))
                CSF_AT(void**, initialFocus, 0)[0x6C / 4])(
                    initialFocus, nullptr));

        if (result)
        {
            result = static_cast<char>(forwardedChild);
            if (!forwardedChild)
            {
                return static_cast<char>(CGuiSlider_ForwardSelfToParent(
                    reinterpret_cast<CGuiControl*>(self->m_pInitialFocus),
                    nullptr));
            }
        }
    }

    return result;
}

HOOK(0x0072A920, CGuiPanel_ResetChildFocusAndAnimation, "auto");
