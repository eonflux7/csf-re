#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, MenuMessageBox_MarkUnavailableControls, 0x00528FD0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
         (void* self, void* edx));

static int __fastcall MenuMessageBox_HandleControlEvent(
    void* self, void* /*edx*/, int event, void* eventData)
{
    if (event == 1)
    {
        if (CSF_AT(std::uint32_t, self, 0x78) & 0x800)
            CSF_AT(std::uint8_t, self, 0x81) = 1;

        MenuMessageBox_MarkUnavailableControls(
            static_cast<std::uint8_t*>(self) - 4, nullptr);
    }
    else if (event == 50 && CSF_AT(std::uint8_t, eventData, 0x38))
    {
        void* target = GetReferencedObject(self, nullptr);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[7])(
            target, nullptr);
    }

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x78);
    if (result)
    {
        CSF_AT(std::uint32_t, self, 0x994) = 0;
    }
    else
    {
        result = CSF_AT(std::uint32_t, self, 0x994);
        CSF_AT(volatile std::uint32_t, self, 0x994) = result;
    }
    return static_cast<int>(result);
}

HOOK(0x0052A610, MenuMessageBox_HandleControlEvent, "auto");
