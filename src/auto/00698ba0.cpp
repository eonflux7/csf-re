#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));

static bool __cdecl CoreFiles_CheckPageAvailabilityWithoutOption(const char* name, char mode)
{
    const std::uint8_t savedOption = CSF_AT(std::uint8_t, g_pOptions, 0x1);
    CSF_AT(std::uint8_t, g_pOptions, 0x1) = 0;

    const bool available = MenuManager_CheckPageAvailability(name, mode);

    CSF_AT(std::uint8_t, g_pOptions, 0x1) = savedOption;
    return available;
}

HOOK(0x00698BA0, CoreFiles_CheckPageAvailabilityWithoutOption, "auto");
