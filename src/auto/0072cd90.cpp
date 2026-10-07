#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int eventId));

static int __fastcall CGuiSlider_SendEvent(
    CGuiSlider* self, void* /*edx*/, int eventId)
{
    switch (eventId)
    {
    case 2:
    case 3:
    case 6:
    {
        void* thumb = reinterpret_cast<void*>(self->m_pThumb);
        if (thumb)
        {
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, thumb, 0)[0x138 / 4])(
                    thumb, nullptr, eventId);
        }

        void* arrowUp = reinterpret_cast<void*>(self->m_pArrowUp);
        if (arrowUp)
        {
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, arrowUp, 0)[0x138 / 4])(
                    arrowUp, nullptr, eventId);
        }

        void* arrowDown = reinterpret_cast<void*>(self->m_pArrowDown);
        if (arrowDown)
        {
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, arrowDown, 0)[0x138 / 4])(
                    arrowDown, nullptr, eventId);
        }
        break;
    }

    case 15:
    {
        void* thumb = reinterpret_cast<void*>(self->m_pThumb);
        if (thumb &&
            (((int(__fastcall*)(void*, void*))
                CSF_AT(void**, thumb, 0)[0x140 / 4])(
                    thumb, nullptr) & 0x10) != 0)
        {
            thumb = reinterpret_cast<void*>(self->m_pThumb);
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, thumb, 0)[0x98 / 4])(
                    thumb, nullptr, &self->field_0160);
        }
        self->field_0168 = 0;
        break;
    }

    case 16:
    {
        void* thumb = reinterpret_cast<void*>(self->m_pThumb);
        if (thumb &&
            (((int(__fastcall*)(void*, void*))
                CSF_AT(void**, thumb, 0)[0x140 / 4])(
                    thumb, nullptr) & 0x10) != 0)
        {
            thumb = reinterpret_cast<void*>(self->m_pThumb);
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, thumb, 0)[0x98 / 4])(
                    thumb, nullptr, &self->field_0158);
            self->field_0168 = 1;
        }
        break;
    }

    default:
        break;
    }

    return CGuiControl_SendEvent(self, nullptr, eventId);
}

HOOK(0x0072CD90, CGuiSlider_SendEvent, "auto");
