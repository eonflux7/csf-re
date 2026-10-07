#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetSelectable, 0x007186D0,
         (CGuiControl* self, void* edx, char selectable));

static char __fastcall CGuiSlider_UpdateConsoleArrowSelectability(
    CGuiSlider* self, void* /*edx*/)
{
    const char console = static_cast<char>(self->m_bConsole);
    if (!console)
        return console;

    if (self->m_pArrowUp)
    {
        const int position =
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);

        CGuiControl_SetSelectable(
            reinterpret_cast<CGuiControl*>(self->m_pArrowUp),
            nullptr, static_cast<char>(position > 0));
    }

    const uint32_t arrowDown = self->m_pArrowDown;
    if (!arrowDown)
        return static_cast<char>(arrowDown);

    const int pageSize =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x278 / 4])(self, nullptr);

    const int position =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x270 / 4])(self, nullptr);

    const int32_t pageEnd = static_cast<int32_t>(
        static_cast<uint32_t>(pageSize) + static_cast<uint32_t>(position));

    const int count =
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x264 / 4])(self, nullptr);

    return static_cast<char>(CGuiControl_SetSelectable(
        reinterpret_cast<CGuiControl*>(self->m_pArrowDown),
        nullptr, static_cast<char>(pageEnd < count)));
}

HOOK(0x0072CEF0, CGuiSlider_UpdateConsoleArrowSelectability, "auto");
