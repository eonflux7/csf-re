#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_OnKeyDown, 0x007184C0,
         (CGuiControl* self, void* edx, int key, int modifiers));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiPanel_HandleKeyDown(
    CGuiPanel* self, void* /*edx*/, int key, int modifiers)
{
    int result =
        ((int (__fastcall*)(void*, void*))
         CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if ((result & 0xFF) == 0)
        return result;

    result = CGuiSlider_IsSelectableAndNotType10(self, nullptr);
    if ((result & 0xFF) == 0)
        return result;

    const uint32_t initialBegin = self->m_childrenBegin;
    const uint32_t initialCount = initialBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->m_childrenEnd - initialBegin) >> 2)
        : 0;

    for (uint32_t index = 0; index < initialCount; ++index)
    {
        CGuiControl* child =
            reinterpret_cast<CGuiControl**>(self->m_childrenBegin)[index];
        ((void (__fastcall*)(void*, void*, int, int))
         CSF_AT(void**, child, 0)[0x08 / 4])(
            child, nullptr, key, modifiers);
    }

    CGuiControl_OnKeyDown(self, nullptr, key, modifiers);

    CGuiControl* current =
        reinterpret_cast<CGuiControl*>(self->field_014c);
    if (!current)
        return 0;

    CGuiControl* nextFocus = nullptr;
    uint32_t attempts = 0;
    const uint32_t navigationKey = static_cast<uint32_t>(key) - 15u;

    for (;;)
    {
        const char* destinationId = nullptr;

        switch (navigationKey)
        {
        case 0:
            if (!self->field_0188)
                break;
            [[fallthrough]];
        case 193:
            if (current)
            {
                destinationId =
                    ((const char* (__fastcall*)(void*, void*))
                     CSF_AT(void**, current, 0)[0x94 / 4])(
                        current, nullptr);
            }
            break;

        case 185:
            if (current)
            {
                destinationId =
                    ((const char* (__fastcall*)(void*, void*))
                     CSF_AT(void**, current, 0)[0x90 / 4])(
                        current, nullptr);
            }
            break;

        case 188:
            if (current)
            {
                destinationId =
                    ((const char* (__fastcall*)(void*, void*))
                     CSF_AT(void**, current, 0)[0x88 / 4])(
                        current, nullptr);
            }
            break;

        case 190:
            if (current)
            {
                destinationId =
                    ((const char* (__fastcall*)(void*, void*))
                     CSF_AT(void**, current, 0)[0x8C / 4])(
                        current, nullptr);
            }
            break;

        default:
            break;
        }

        if (destinationId && *destinationId)
        {
            current =
                ((CGuiControl* (__fastcall*)(void*, void*, const char*))
                 CSF_AT(void**, self, 0)[0xFC / 4])(
                    self, nullptr, destinationId);

            if (current)
            {
                const int shown =
                    ((int (__fastcall*)(void*, void*))
                     CSF_AT(void**, current, 0)[0x6C / 4])(
                        current, nullptr);

                if ((shown & 0xFF) != 0)
                {
                    const int selectable =
                        CGuiSlider_IsSelectableAndNotType10(
                            current, nullptr);
                    if ((selectable & 0xFF) != 0)
                        nextFocus = current;
                }
            }
        }

        ++attempts;
        const uint32_t childrenBegin = self->m_childrenBegin;
        if (childrenBegin)
        {
            const uint32_t count = static_cast<uint32_t>(
                static_cast<int32_t>(
                    self->m_childrenEnd - childrenBegin) >> 2);

            if (attempts < count && !nextFocus)
            {
                if (!destinationId || !*destinationId)
                    return 0;
                continue;
            }
        }

        if (!nextFocus)
            return 0;

        ((void (__fastcall*)(void*, void*, CGuiControl*))
         CSF_AT(void**, self, 0)[0x158 / 4])(
            self, nullptr, nextFocus);

        void* builderState =
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0);
        void* notificationTarget = CSF_AT(void*, builderState, 4);

        if (notificationTarget)
        {
            return
                ((int (__fastcall*)(void*, void*, int))
                 CSF_AT(void**, notificationTarget, 0)[0x24 / 4])(
                    notificationTarget, nullptr, 0);
        }

        return reinterpret_cast<int>(self);
    }
}

HOOK(0x0072B2E0, CGuiPanel_HandleKeyDown, "auto");
