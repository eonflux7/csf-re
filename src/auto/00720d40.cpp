#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* drawable));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, int* drawable));
CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int eventId));

static int __fastcall CGuiStatic_SendEvent(
    CGuiStatic* self, void* /*edx*/, int eventId)
{
    if (eventId >= 2)
    {
        if (eventId <= 3)
        {
            CGuiStatic_CopyPropertiesToObject(
                self, nullptr, reinterpret_cast<int*>(self->field_0138));
            CGuiStatic_CopyStateToTarget(
                self, nullptr, reinterpret_cast<int*>(self->m_pText));
        }
        else if (eventId == 6)
        {
            alignas(4) std::uint8_t auxiliaryValueBuffer[16];
            alignas(4) std::uint8_t textValueBuffer[16];

            void* auxiliaryObject =
                reinterpret_cast<void*>(self->field_0138);
            if (auxiliaryObject)
            {
                void** auxiliaryVtable = CSF_AT(void**, auxiliaryObject, 0);
                int value =
                    ((int (__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, self, 0)[0x228 / 4])(
                            self, nullptr, auxiliaryValueBuffer);

                ((void (__fastcall*)(void*, void*, int))
                    auxiliaryVtable[0x68 / 4])(
                        reinterpret_cast<void*>(self->field_0138),
                        nullptr, value);
            }

            void* textObject = reinterpret_cast<void*>(self->m_pText);
            if (textObject)
            {
                void** textVtable = CSF_AT(void**, textObject, 0);
                int value =
                    ((int (__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, self, 0)[0x228 / 4])(
                            self, nullptr, textValueBuffer);

                ((void (__fastcall*)(void*, void*, int))
                    textVtable[0x60 / 4])(
                        reinterpret_cast<void*>(self->m_pText),
                        nullptr, value);
            }
        }
    }

    return CGuiControl_SendEvent(self, nullptr, eventId);
}

HOOK(0x00720D40, CGuiStatic_SendEvent, "auto");
