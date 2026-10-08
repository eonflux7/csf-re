#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, MenuMpPause_GetEventReceiver, 0x006683F0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuMpPause_ApplyEventAction, 0x00535900,
         (void* self, void* edx));

static void __fastcall MenuMpPause_HandleEvent(
    void* self, void* /*edx*/, std::int32_t eventCode, void* eventData)
{
    if (CSF_AT(std::uint32_t, self, 0x10) == 1)
        return;

    if (eventCode == 50 && CSF_AT(std::uint8_t, eventData, 0x38) != 0)
    {
        void* receiver = MenuMpPause_GetEventReceiver(self, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, receiver, 0)[7])(receiver, nullptr);
    }

    void* relatedObject = CSF_AT(void*, self, 0x20);
    if (relatedObject && CSF_AT(std::uint8_t, relatedObject, 0x11) != 0)
    {
        if (eventCode != 1)
            return;
    }
    else if (eventCode == 50)
    {
        return;
    }

    MenuMpPause_ApplyEventAction(
        static_cast<std::uint8_t*>(self) - 4, nullptr);
}

HOOK(0x00535B90, MenuMpPause_HandleEvent, "auto");
