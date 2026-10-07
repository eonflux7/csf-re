#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_ProcessInput, 0x00718300,
         (CGuiControl* self, void* edx, void* input));
CSF_FUNC(int, __fastcall, CGuiControl_IsInteractionAllowed, 0x00718700,
         (CGuiControl* self, void* edx));

static char __fastcall CGuiSlider_ProcessInput(
    CGuiSlider* self, void* /*edx*/, void* input)
{
    char result =
        ((char(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (!result)
        return result;

    std::uint32_t pointerPosition[2] = {
        CSF_AT(std::uint32_t, input, 0x14),
        CSF_AT(std::uint32_t, input, 0x18)
    };

    result = CGuiControl_ProcessInput(self, nullptr, input);

    CGuiControl* part = reinterpret_cast<CGuiControl*>(self->m_pThumb);
    if (part)
    {
        if (((char(__fastcall*)(void*, void*, std::uint32_t*))
                 CSF_AT(void**, part, 0)[0x154 / 4])(
                    part, nullptr, pointerPosition))
        {
            part = reinterpret_cast<CGuiControl*>(self->m_pThumb);
            if (static_cast<std::uint8_t>(
                    CGuiControl_IsInteractionAllowed(part, nullptr)))
            {
                part = reinterpret_cast<CGuiControl*>(self->m_pThumb);
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, part, 0)[0x144 / 4])(part, nullptr, 1);
            }
        }

        part = reinterpret_cast<CGuiControl*>(self->m_pThumb);
        ((char(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, part, 0)[0x14 / 4])(part, nullptr, input);

        std::uint32_t* thumbPosition;
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) & 1) != 0)
        {
            part = reinterpret_cast<CGuiControl*>(self->m_pThumb);
            thumbPosition =
                ((std::uint32_t*(__fastcall*)(void*, void*))
                     CSF_AT(void**, part, 0)[0x9C / 4])(part, nullptr);
        }
        else
        {
            thumbPosition = &self->field_0158;
        }

        self->field_0158 = thumbPosition[0];
        self->field_015c = thumbPosition[1];
        result = static_cast<char>(
            reinterpret_cast<std::uintptr_t>(thumbPosition));
    }

    part = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
    if (part)
    {
        if (((char(__fastcall*)(void*, void*, std::uint32_t*))
                 CSF_AT(void**, part, 0)[0x154 / 4])(
                    part, nullptr, pointerPosition))
        {
            part = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
            if (static_cast<std::uint8_t>(
                    CGuiControl_IsInteractionAllowed(part, nullptr)))
            {
                part = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, part, 0)[0x144 / 4])(part, nullptr, 1);
            }
        }

        part = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
        result = ((char(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, part, 0)[0x14 / 4])(
                         part, nullptr, input);
    }

    part = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
    if (part)
    {
        if (((char(__fastcall*)(void*, void*, std::uint32_t*))
                 CSF_AT(void**, part, 0)[0x154 / 4])(
                    part, nullptr, pointerPosition))
        {
            part = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
            if (static_cast<std::uint8_t>(
                    CGuiControl_IsInteractionAllowed(part, nullptr)))
            {
                part = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, part, 0)[0x144 / 4])(part, nullptr, 1);
            }
        }

        part = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
        result = ((char(__fastcall*)(void*, void*, void*))
                      CSF_AT(void**, part, 0)[0x14 / 4])(
                         part, nullptr, input);
    }

    return result;
}

HOOK(0x0072DA00, CGuiSlider_ProcessInput, "auto");
