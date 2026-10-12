#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, sub_4FA800, 0x004FA800, (uint16_t* a1, uint16_t* a2));
CSF_FUNC(char, __fastcall, sub_4FAEA0, 0x004FAEA0, (void* self, void* edx, int a2, wchar_t* Source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static char __fastcall S_004faea0_ToggleConsoleOption(
    S_004faea0* self, void* /*edx*/, uint16_t* arguments)
{
    int state = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(state), 0xD0))
        return static_cast<char>(state);

    alignas(8) uint8_t argumentBytes[0xC8];
    auto* argument = reinterpret_cast<uint16_t*>(argumentBytes);
    sub_4FA800(arguments, argument);

    bool enabled;
    if (argument[0])
        enabled = argument[0] != 48;
    else
        enabled = CSF_AT(uint8_t,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x90C) == 0;

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x90C) =
        enabled;

    const char* message = enabled
        ? CSF_STR(0x0083F2A0, "CG01")
        : CSF_STR(0x0083F298, "CG02");
    auto* displayText = reinterpret_cast<wchar_t*>(
        Text_ConvertToDisplayEncoding(reinterpret_cast<int>(message), 16));

    (void)CSF_VTBL(0x004FAEA0);
    return sub_4FAEA0(self, nullptr, 2, displayText);
}

HOOK(0x004FB110, S_004faea0_ToggleConsoleOption, "auto");
