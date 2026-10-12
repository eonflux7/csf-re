#include <cstdint>
#include "globals.h"
#include "hook.h"

static char __fastcall DevConsole_UpdateBlink(void* self, void* /*edx*/)
{
    const auto enabled = CSF_AT(uint8_t, self, 0x10);
    if (!enabled)
        return static_cast<char>(enabled);

    const auto countdown = CSF_AT(uint32_t, self, 0x15C);
    if (countdown != 0)
    {
        const auto remaining = countdown - 1;
        CSF_AT(uint32_t, self, 0x15C) = remaining;
        return static_cast<char>(remaining);
    }

    const auto visible = static_cast<uint8_t>(CSF_AT(uint8_t, self, 0x158) == 0);
    CSF_AT(uint32_t, self, 0x15C) = 10;
    CSF_AT(uint8_t, self, 0x158) = visible;
    return static_cast<char>(visible);
}

HOOK(0x004FA7C0, DevConsole_UpdateBlink, "auto");
