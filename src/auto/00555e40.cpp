#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(char*, __fastcall, sub_5557C0, 0x005557C0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __stdcall, sub_555830, 0x00555830, (void* a1, int arg4, int a2, int a4, unsigned int a5));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void*, __fastcall, sub_696F00, 0x00696F00, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, sub_696F20, 0x00696F20, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(void*, __fastcall, sub_697150, 0x00697150, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, sub_73CD30, 0x0073CD30, (void* self, void* edx));
CSF_FUNC(int, __stdcall, MpSpawnPoints_FindPlayerById, 0x0073E2C0, (int playerId));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_7411A0, 0x007411A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_74A190, 0x0074A190, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, sub_74A520, 0x0074A520, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall S_00555e40_FormatAndQueueChatMessage(
    S_00555e40* self, void* /*edx*/, int messageAddress)
{
    auto* message = reinterpret_cast<std::uint8_t*>(messageAddress);
    alignas(4) std::uint8_t generatedText[0x1C];
    alignas(4) std::uint8_t combinedText[0x1C];
    alignas(4) std::uint8_t prefix[0x28];

    if (CSF_AT(std::uint8_t, message, 0x28))
    {
        void* text = sub_555830(
            generatedText,
            CSF_AT(int, message, 0x2C),
            CSF_AT(int, message, 0x30),
            CSF_AT(int, message, 0x34),
            CSF_AT(unsigned int, message, 0x38));
        AssignStringRange_488830(message + 8, nullptr, text, 0, 0xFFFFFFFF);
        if (CSF_AT(std::uint32_t, generatedText, 0x18) >= 8)
        {
            if (CSF_AT(void*, generatedText, 0x04))
                Mem_FreeDebug(CSF_AT(void*, generatedText, 0x04));
        }
    }

    bool localPlayerActive = false;
    int localTeam = 0;
    if (self->field_0c8c)
    {
        const std::uint32_t playerInterface =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0c8c), 0x18);
        if (playerInterface && playerInterface - 1712 != 0)
        {
            auto* localPlayer = reinterpret_cast<std::uint8_t*>(playerInterface - 1712);
            localPlayerActive = static_cast<std::uint8_t>(
                sub_7411A0(localPlayer, nullptr)) == 0;
            localTeam = CSF_AT(std::int8_t, localPlayer, 0x443);
        }
    }

    bool senderActive = true;
    int senderTeam = 0;
    FormatterSetPattern(prefix, nullptr, reinterpret_cast<const char*>(&g_byte_82E65C));

    if (CSF_AT(int, message, 0x00) != -1)
    {
        const int senderId = CSF_AT(int, message, 0x00);
        GetGlobalDword_73C020();
        const int playerInterface = MpSpawnPoints_FindPlayerById(senderId);
        if (!playerInterface || playerInterface == 1712)
        {
            const int manager = GetGlobalDword_73C020();
            const int playerIndex = sub_74A190(
                CSF_AT(void*, reinterpret_cast<void*>(manager), 0xC8),
                nullptr, CSF_AT(int, message, 0x00));
            if (playerIndex == -1)
                goto PrefixFinished;

            const int lookupManager = GetGlobalDword_73C020();
            char* player = sub_74A520(
                CSF_AT(void*, reinterpret_cast<void*>(lookupManager), 0xC8),
                nullptr, playerIndex);
            if (!player)
                goto PrefixFinished;
            AssignStringRange_488830(prefix, nullptr, player + 4, 0, 0xFFFFFFFF);
        }
        else
        {
            auto* player = reinterpret_cast<std::uint8_t*>(
                static_cast<std::uint32_t>(playerInterface) - 1712);
            senderActive = static_cast<std::uint8_t>(
                sub_7411A0(player, nullptr)) == 0;
            senderTeam = CSF_AT(std::int8_t, player, 0x443);
            void* playerName = MenuMpAccounts_GetPlayerName(player, nullptr);
            AssignStringRange_488830(prefix, nullptr, playerName, 0, 0xFFFFFFFF);

            if (!senderActive)
            {
                sub_697150(prefix, nullptr,
                    reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C4, "["))));
                auto* entry = reinterpret_cast<wchar_t*>(
                    MenuMessageBox_ResolveEntryByKey(1668511536, 2));
                sub_696F20(prefix, nullptr, entry);
                sub_697150(prefix, nullptr,
                    reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C0, "]"))));
            }
        }

        if (!senderTeam)
        {
            sub_697150(prefix, nullptr,
                reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C4, "["))));
            auto* entry = reinterpret_cast<wchar_t*>(
                MenuMessageBox_ResolveEntryByKey(1668511538, 2));
            sub_696F20(prefix, nullptr, entry);
            sub_697150(prefix, nullptr,
                reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C0, "]"))));
        }

        if ((CSF_AT(std::uint8_t, message, 0x24) & 2) != 0)
        {
            sub_697150(prefix, nullptr,
                reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C4, "["))));
            auto* entry = reinterpret_cast<wchar_t*>(
                MenuMessageBox_ResolveEntryByKey(1668511537, 2));
            sub_696F20(prefix, nullptr, entry);
            sub_697150(prefix, nullptr,
                reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446C0, "]"))));
        }
        sub_697150(prefix, nullptr,
            reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x008446BC, ":"))));
    }

PrefixFinished:
    CSF_AT(std::uint32_t, combinedText, 0x18) = 7;
    CSF_AT(int, combinedText, 0x14) = 0;
    CSF_AT(std::uint16_t, combinedText, 0x04) = 0;
    AssignStringRange_488830(combinedText, nullptr, prefix, 0, 0xFFFFFFFF);
    sub_696F00(combinedText, nullptr, reinterpret_cast<uint32_t*>(message + 8));
    AssignStringRange_488830(message + 8, nullptr, combinedText, 0, 0xFFFFFFFF);

    if (!localPlayerActive || senderActive)
        senderActive = true;

    const int colorTeam = senderTeam;
    const bool visibleTeam = !localTeam || senderTeam;
    const bool matchingTeam =
        senderTeam == localTeam || (CSF_AT(std::uint8_t, message, 0x24) & 2) == 0;
    bool privateMessage = false;
    bool shouldQueue = true;

    if (CSF_AT(int, message, 0x04) == -1)
    {
        if ((CSF_AT(std::uint8_t, message, 0x24) & 1) == 0 &&
            (!senderActive || !visibleTeam || !matchingTeam))
            shouldQueue = false;
    }
    else
    {
        const int manager = GetGlobalDword_73C020();
        const int localId = sub_73CD30(reinterpret_cast<void*>(manager), nullptr);
        if (CSF_AT(int, message, 0x04) != localId)
            shouldQueue = false;
        else
            privateMessage = true;
    }

    if (shouldQueue)
    {
        if ((CSF_AT(std::uint8_t, message, 0x24) & 1) != 0)
        {
            CSF_AT(std::int8_t, message, 0x3C) = -28;
            CSF_AT(std::int8_t, message, 0x3D) = -78;
            CSF_AT(std::int8_t, message, 0x3E) = 98;
        }
        else if (privateMessage)
        {
            CSF_AT(std::int8_t, message, 0x3C) = -124;
            CSF_AT(std::int8_t, message, 0x3D) = -88;
            CSF_AT(std::int8_t, message, 0x3E) = 85;
        }
        else if (colorTeam == 1)
        {
            CSF_AT(std::int8_t, message, 0x3C) = -3;
            CSF_AT(std::int8_t, message, 0x3D) = -123;
            CSF_AT(std::int8_t, message, 0x3E) = 112;
        }
        else if (colorTeam == 2)
        {
            CSF_AT(std::int8_t, message, 0x3E) = -34;
            CSF_AT(std::int8_t, message, 0x3C) = -93;
            CSF_AT(std::int8_t, message, 0x3D) = -93;
        }
        else if (colorTeam == 0)
        {
            CSF_AT(std::int8_t, message, 0x3E) = -76;
            CSF_AT(std::int8_t, message, 0x3C) = -76;
            CSF_AT(std::int8_t, message, 0x3D) = -76;
        }

        sub_5557C0(
            reinterpret_cast<std::uint8_t*>(self) +
                64 * CSF_AT(int, self, 0xC84) + 4,
            nullptr, messageAddress);
        const int nextIndex = CSF_AT(int, self, 0xC84) + 1;
        CSF_AT(int, self, 0x141C) = 0;
        CSF_AT(int, self, 0xC84) = nextIndex;
        if (nextIndex == 50)
            CSF_AT(int, self, 0xC84) = 0;
    }

    if (CSF_AT(std::uint32_t, combinedText, 0x18) >= 8 &&
        CSF_AT(void*, combinedText, 0x04))
        Mem_FreeDebug(CSF_AT(void*, combinedText, 0x04));

    void* result = reinterpret_cast<void*>(CSF_AT(std::uint32_t, prefix, 0x18));
    CSF_AT(std::uint32_t, combinedText, 0x18) = 7;
    CSF_AT(int, combinedText, 0x14) = 0;
    CSF_AT(std::uint16_t, combinedText, 0x04) = 0;
    if (CSF_AT(std::uint32_t, prefix, 0x18) >= 8)
    {
        result = CSF_AT(void*, prefix, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00555E40, S_00555e40_FormatAndQueueChatMessage, "auto");
