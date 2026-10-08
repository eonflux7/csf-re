#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static void __fastcall MenuMessageBox_MarkUnavailableControls(
    void* self, void* /*edx*/)
{
    void* control = CSF_AT(void*, self, 0x30);
    if (!control)
        return;
    if (!CSF_AT(void*, self, 0x38))
        return;
    if (!CSF_AT(void*, self, 0x2C))
        return;

    if (!((unsigned char (__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0xAC / 4])(control, nullptr))
    {
        CSF_AT(std::uint8_t, self, 0x86) = 1;
        return;
    }

    control = CSF_AT(void*, self, 0x38);
    if (!((unsigned char (__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0xAC / 4])(control, nullptr))
    {
        CSF_AT(std::uint8_t, self, 0x88) = 1;
        return;
    }

    control = CSF_AT(void*, self, 0x2C);
    if (!((unsigned char (__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0xAC / 4])(control, nullptr))
    {
        CSF_AT(std::uint8_t, self, 0x82) = 1;
    }
}

HOOK(0x00528FD0, MenuMessageBox_MarkUnavailableControls, "auto");
