#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall MenuMpOnline_ActivateMenuControls(
    void* self, void* /*edx*/)
{
    void* control = CSF_AT(void*, self, 0x1C);
    if (control)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[0x68 / 4])(
                control, nullptr, 1, 1, 1);
    }

    control = CSF_AT(void*, self, 0x20);
    if (control)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[0x68 / 4])(
                control, nullptr, 1, 1, 1);
    }

    control = CSF_AT(void*, self, 0x24);
    if (control)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[0x68 / 4])(
                control, nullptr, 1, 1, 1);
    }

    control = CSF_AT(void*, self, 0x28);
    if (control)
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(
                control, nullptr, 1);
    }
    return 0;
}

HOOK(0x0055F7E0, MenuMpOnline_ActivateMenuControls, "auto");
