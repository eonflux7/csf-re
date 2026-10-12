#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __fastcall, MpGameModes_GetBooleanSetting, 0x00744240, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, MpGameModes_GetFloatSetting, 0x007442A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpGameModes_GetIntegerSetting, 0x00744480, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpGameModes_GetIdentifierSetting, 0x00744580, (void* self, void* edx, int a2));

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void* MpGameModes_GetSettingTable(void* self, std::uint32_t offset)
{
    const std::uint32_t ownerLink = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t owner = ownerLink ? ownerLink - 0xD8 : 0;
    return reinterpret_cast<void*>(owner + offset);
}

static int __fastcall MpGameModes_CopyMatchSettings(
    void* self, void* /*edx*/, std::uint32_t* settings)
{
    GetGlobal8B0688();
    const bool online = g_OnlineMatchFlag != 0;
    const auto defaultsAddress =
        static_cast<std::uint32_t>(GetValue_8B08F8()) + (online ? 172 : 24);
    std::memcpy(settings, reinterpret_cast<const void*>(defaultsAddress), 0x94);

    settings[0] = MpGameModes_GetIdentifierSetting(
        MpGameModes_GetSettingTable(self, 0x44), nullptr, 0);
    settings[1] = MpGameModes_GetIdentifierSetting(
        MpGameModes_GetSettingTable(self, 0x44), nullptr, 1);
    settings[2] = MpGameModes_GetIdentifierSetting(
        MpGameModes_GetSettingTable(self, 0x44), nullptr, 2);

    settings[3] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 3);
    settings[4] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 4);
    settings[5] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 5);
    settings[6] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 6);
    settings[7] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 7);
    settings[8] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 8);
    settings[9] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 9);
    settings[10] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 10);
    settings[11] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 11);
    settings[12] = MpGameModes_GetIntegerSetting(
        MpGameModes_GetSettingTable(self, 0xA8), nullptr, 12);

    CSF_AT(float, settings, 0x34) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 13));
    CSF_AT(float, settings, 0x38) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 14));
    CSF_AT(float, settings, 0x3C) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 15));
    CSF_AT(float, settings, 0x40) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 16));
    CSF_AT(float, settings, 0x44) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 17));
    CSF_AT(float, settings, 0x48) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 18));
    CSF_AT(float, settings, 0x4C) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 19));

    settings[20] = MpGameModes_GetBooleanSetting(
        MpGameModes_GetSettingTable(self, 0x30), nullptr, 20);
    settings[21] = MpGameModes_GetBooleanSetting(
        MpGameModes_GetSettingTable(self, 0x30), nullptr, 21);

    CSF_AT(float, settings, 0x58) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 22));
    CSF_AT(float, settings, 0x5C) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 23));
    CSF_AT(float, settings, 0x60) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 24));
    CSF_AT(float, settings, 0x64) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 25));
    CSF_AT(float, settings, 0x68) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 26));
    CSF_AT(float, settings, 0x6C) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 27));
    CSF_AT(float, settings, 0x70) = static_cast<float>(MpGameModes_GetFloatSetting(
        MpGameModes_GetSettingTable(self, 0x58), nullptr, 28));

    settings[29] = MpGameModes_GetBooleanSetting(
        MpGameModes_GetSettingTable(self, 0x30), nullptr, 29);
    const int result = MpGameModes_GetBooleanSetting(
        MpGameModes_GetSettingTable(self, 0x30), nullptr, 30);
    settings[30] = result;
    return result;
}

HOOK(0x0054F680, MpGameModes_CopyMatchSettings, "auto");
