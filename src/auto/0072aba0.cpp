#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* quad));
CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int eventId));

static int __fastcall CGuiPanel_SendEvent(
    CGuiPanel* self, void* /*edx*/, int eventId)
{
    if (eventId >= 2 && (eventId <= 3 || eventId == 6))
    {
        if (eventId <= 3)
        {
            CGuiStatic_CopyPropertiesToObject(
                self, nullptr, reinterpret_cast<int*>(self->m_pBackgroundQuad));
        }
        else
        {
            void* background = reinterpret_cast<void*>(self->m_pBackgroundQuad);
            if (background)
            {
                void** backgroundVtable = CSF_AT(void**, background, 0);
                alignas(4) std::uint8_t rectangleBuffer[16];

                int rectangle =
                    reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
                        CSF_AT(void**, self, 0)[0x228 / 4])(
                            self, nullptr, rectangleBuffer);

                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    backgroundVtable[0x68 / 4])(
                        reinterpret_cast<void*>(self->m_pBackgroundQuad),
                        nullptr, rectangle);
            }
        }

        const std::uint32_t childrenBegin = self->field_0138;
        std::uint32_t childCount = 0;
        if (childrenBegin)
        {
            childCount = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    self->field_013c - childrenBegin) >> 2);
        }

        for (std::uint32_t index = 0; index < childCount; ++index)
        {
            void* child =
                reinterpret_cast<void**>(self->field_0138)[index];
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[0x138 / 4])(
                    child, nullptr, eventId);
        }
    }

    return CGuiControl_SendEvent(self, nullptr, eventId);
}

HOOK(0x0072ABA0, CGuiPanel_SendEvent, "auto");
