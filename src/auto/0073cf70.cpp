#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __cdecl, NetSession_GetSettings, 0x00668B70, ());
CSF_FUNC(char, __cdecl, NetSession_RequestConnection, 0x00669760,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, NetSession_SetSetting, 0x00680D30,
         (char* self, void* edx, const char* name, const char* value));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620,
         (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(char, __fastcall, NetSession_ResetConnection, 0x0073C730,
         (void* self, void* edx, int a3));
CSF_FUNC(char, __cdecl, NetSession_InitializeConnection, 0x007A0150, ());

CSF_GLOBAL(char, g_MaxPlayersSettingValue, 0x008445D8);
CSF_GLOBAL(std::uint8_t, g_ConnectionRequestFlag, 0x008BBF98);

static char __fastcall NetSession_TryConnect(void* self, void* /*edx*/)
{
    const std::int32_t state = CSF_AT(std::int32_t, self, 0x98);
    if (state == 6 || state == 0)
        return static_cast<char>(state);

    if (!CSF_AT(std::uint8_t, self, 0x14F))
    {
        NetSession_InitializeConnection();
        CSF_AT(std::uint8_t, self, 0x14F) = 1;
    }

    alignas(8) char addressText[0x100];
    std::memset(addressText, 0, sizeof(addressText));

    std::uint16_t* source;
    if (CSF_AT(std::uint32_t, self, 0x40) < 8u)
        source = reinterpret_cast<std::uint16_t*>(
            static_cast<std::uint8_t*>(self) + 0x2C);
    else
        source = CSF_AT(std::uint16_t*, self, 0x2C);

    ConvertWideTextToNarrow(source, addressText, 256);

    // The conversion's three arguments remain on the original stack.
    char result = NetSession_RequestConnection(
        static_cast<int>(reinterpret_cast<std::intptr_t>(addressText)),
        1,
        static_cast<int>(reinterpret_cast<std::intptr_t>(source)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(addressText)),
        256);

    if (result)
    {
        NetSession_ResetConnection(self, nullptr, 0);
        if (NetSession_GetSettings())
        {
            char* settings = NetSession_GetSettings();
            result = NetSession_SetSetting(
                settings, nullptr, CSF_STR(0x00867624, "MaxJugs"),
                &g_MaxPlayersSettingValue);
        }
        else
        {
            result = 0;
        }

        CSF_AT(std::int32_t, self, 0x98) = 6;
        CSF_AT(std::uint8_t, self, 0xFC) = 1;
        CSF_AT(std::uint8_t, self, 0x104) = 0;
    }
    else
    {
        g_ConnectionRequestFlag = 0;
    }

    return result;
}

HOOK(0x0073CF70, NetSession_TryConnect, "auto");
