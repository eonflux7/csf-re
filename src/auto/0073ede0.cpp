#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(char, __cdecl, NetSession_WaitForSettings, 0x006476A0, (int a1));
CSF_FUNC(char, __fastcall, NetSession_SetSettingsPair, 0x00651FC0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, NetSession_InitializeSettingsHandle, 0x00652290, (uint32_t* a1));
CSF_FUNC(int, __cdecl, NetSession_SetTransportOption, 0x0068EC30, (int a1));
CSF_FUNC(char, __fastcall, NetSession_SetTransportPair, 0x0068EC40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __stdcall, NetSession_GetSettingsObject, 0x0073E830, (uint32_t* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(int, g_SettingsWaitDuration, 0x0085807C);
CSF_GLOBAL(uint8_t, g_TransportFlag, 0x008B99B4);
CSF_GLOBAL(uint8_t, g_TransportSettings, 0x008BAC1C);

static int __fastcall NetSession_ApplyConnectionSettings(void* self, void* /*edx*/, char alternate)
{
    int settings[37];
    const auto* settingsBase =
        reinterpret_cast<const uint8_t*>(GetValue_8B08F8());
    const void* source = settingsBase + (alternate ? 24 : 172);
    std::memcpy(settings, source, sizeof(settings));

    NetSession_SetTransportOption(settings[31]);
    NetSession_SetTransportPair(&g_TransportSettings, nullptr,
                               settings[32], settings[33]);
    g_TransportFlag = settings[34] == 1;

    alignas(8) uint8_t handleBytes[0x4];
    auto* handle = reinterpret_cast<uint32_t*>(handleBytes);
    NetSession_InitializeSettingsHandle(handle);
    int settingsObject = NetSession_GetSettingsObject(handle);
    NetSession_SetSettingsPair(reinterpret_cast<void*>(settingsObject), nullptr,
                              settings[35], settings[36]);
    NetSession_WaitForSettings(g_SettingsWaitDuration);

    CSF_AT(uint8_t, self, 0x10C) = alternate == 0;
    int result = GetGlobalValue_75FDF0();
    CSF_AT(uint32_t, self, 0x110) =
        CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0xC8);
    return result;
}

HOOK(0x0073EDE0, NetSession_ApplyConnectionSettings, "auto");
