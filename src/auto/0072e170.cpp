#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_OnKeyDown, 0x007184C0,
         (CGuiControl* self, void* edx, int key, int keyData));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (CGuiSlider* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiSlider_RedirectConsoleArrowSelection, 0x0072D760,
         (CGuiSlider* self, void* edx));

static char __fastcall CGuiSlider_OnKeyDown(
    CGuiSlider* self, void* /*edx*/, int key, int keyData)
{
    char result =
        ((char(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (!result)
        return result;

    result =
        ((char(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
    if (!result)
        return result;

    result = static_cast<char>(
        CGuiSlider_IsSelectableAndNotType10(self, nullptr));
    if (!result)
        return result;

    if (key == 28 || key == 156)
    {
        void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
        if (arrowUp &&
            ((char(__fastcall*)(void*, void*))
                 CSF_AT(void**, arrowUp, 0)[0xBC / 4])(arrowUp, nullptr))
        {
            const std::uint32_t position = self->m_pos - 1u;
            ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                 CSF_AT(void**, self, 0)[0x26C / 4])(
                self, nullptr, position, 1);
        }

        void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
        if (arrowDown &&
            ((char(__fastcall*)(void*, void*))
                 CSF_AT(void**, arrowDown, 0)[0xBC / 4])(arrowDown, nullptr))
        {
            const std::uint32_t position = self->m_pos + 1u;
            ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                 CSF_AT(void**, self, 0)[0x26C / 4])(
                self, nullptr, position, 1);
        }
    }
    else
    {
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, self, 0)[0x284 / 4])(self, nullptr, key);
    }

    void* thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb &&
        ((char(__fastcall*)(void*, void*))
             CSF_AT(void**, thumb, 0)[0xBC / 4])(thumb, nullptr))
    {
        const std::uint32_t orientation = self->m_bHorizontal;
        const std::uint32_t previousPosition = self->m_pos;

        if (orientation == 0)
        {
            if (key == 200)
            {
                ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                     CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, previousPosition - 1u, 0);
            }
            else if (key == 208)
            {
                ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                     CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, previousPosition + 1u, 0);
            }
        }
        else if (orientation == 1)
        {
            if (key == 203)
            {
                ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                     CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, previousPosition - 1u, 0);
            }
            else if (key == 205)
            {
                ((void(__fastcall*)(void*, void*, std::uint32_t, int))
                     CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, previousPosition + 1u, 0);
            }
        }

        if (previousPosition != self->m_pos)
        {
            void* table = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* feedback = CSF_AT(void*, table, 4);
            if (feedback)
            {
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, feedback, 0)[0x24 / 4])(
                    feedback, nullptr, 0);
            }

            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
        }
    }

    void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
    if (arrowUp)
    {
        ((char(__fastcall*)(void*, void*, int, int))
             CSF_AT(void**, arrowUp, 0)[0x08 / 4])(
            arrowUp, nullptr, key, keyData);
    }

    void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrowDown)
    {
        ((char(__fastcall*)(void*, void*, int, int))
             CSF_AT(void**, arrowDown, 0)[0x08 / 4])(
            arrowDown, nullptr, key, keyData);
    }

    thumb = reinterpret_cast<void*>(self->m_pThumb);
    if (thumb)
    {
        ((char(__fastcall*)(void*, void*, int, int))
             CSF_AT(void**, thumb, 0)[0x08 / 4])(
            thumb, nullptr, key, keyData);
    }

    CGuiControl_OnKeyDown(self, nullptr, key, keyData);
    return CGuiSlider_RedirectConsoleArrowSelection(self, nullptr);
}

HOOK(0x0072E170, CGuiSlider_OnKeyDown, "auto");
