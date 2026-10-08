#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Trivial 28-byte argument representation: the formatter consumes this string
// by value and releases any heap storage it owns.
struct PlayerIconStringArgument {
    unsigned char bytes[28];
};
static_assert(sizeof(PlayerIconStringArgument) == 28);

CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void*, __fastcall, MenuMpAccounts_FindAccountByName, 0x0055BCB0,
         (void* self, void* edx, char* name));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620,
         (const std::uint16_t* source, char* destination, unsigned int count));
CSF_FUNC(void, __cdecl, AppendWideString, 0x00695700,
         (std::uint16_t* destination, const std::uint16_t* source));
CSF_FUNC(void*, __fastcall, FormatterSetPattern, 0x00697100,
         (void* self, void* edx, const void* text));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80,
         (void* destination, PlayerIconStringArgument format,
          int icon, int formattingMode));
CSF_FUNC(std::uint8_t, __cdecl, IsAlternatePlayerState, 0x00743920,
         (int playerId));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(std::uint8_t, g_EmptyPlayerIconText, 0x0082E65C);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static int __stdcall CMenuPlayerList_BuildPlayerStatusIcons(
    const std::uint16_t* playerName,
    int connectionId,
    int playerId,
    std::uint16_t* output,
    int /*unused*/)
{
    if (!output)
        return 0;

    *output = 0;
    char narrowName[16] = {};
    int accountIcon = -1;
    int connectionIcon = -1;
    int flags = 0;

    ConvertWideTextToNarrow(playerName, narrowName, 16);

    void* state = GetGlobalState_6A6D70();
    void* accounts =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0xF4 / 4])(state, nullptr);
    void* account =
        MenuMpAccounts_FindAccountByName(accounts, nullptr, narrowName);

    if (account) {
        switch (CSF_AT(std::uint32_t, account, 0x90)) {
        case 0:
            if (CSF_AT(std::uint32_t, account, 0x94))
                accountIcon = 19;
            flags = 1;
            break;
        case 1:
            accountIcon = 24;
            flags = 4;
            break;
        case 2:
            accountIcon = 20;
            flags = 2;
            break;
        default:
            break;
        }
    }

    void* network = CSF_AT(void*, GetGlobal8B0688(), 0x1484);
    using StatusQuery = std::uint8_t (__fastcall*)(void*, void*, int);

    if (reinterpret_cast<StatusQuery>(
            CSF_AT(void**, network, 0)[0x18 / 4])(
                network, nullptr, playerId)) {
        connectionIcon = 26;
        flags |= 0x10;
    } else if (reinterpret_cast<StatusQuery>(
                   CSF_AT(void**, network, 0)[0x20 / 4])(
                       network, nullptr, connectionId)) {
        connectionIcon = IsAlternatePlayerState(playerId) ? 30 : 25;
        flags |= 8;
    } else {
        flags |= 0x20;
        if (!reinterpret_cast<StatusQuery>(
                CSF_AT(void**, network, 0)[0x28 / 4])(
                    network, nullptr, connectionId) &&
            !reinterpret_cast<StatusQuery>(
                CSF_AT(void**, network, 0)[0x24 / 4])(
                    network, nullptr, connectionId)) {
            connectionIcon = 29;
        }
    }

    PlayerIconStringArgument accountText;
    PlayerIconStringArgument connectionText;
    FormatterSetPattern(
        &accountText, nullptr, &g_EmptyPlayerIconText);
    FormatterSetPattern(
        &connectionText, nullptr, &g_EmptyPlayerIconText);

    if (accountIcon != -1) {
        const int formattingMode = g_NumberFormattingMode;
        PlayerIconStringArgument format;
        FormatterSetPattern(
            &format, nullptr, CSF_STR(0x008456C8, "@@{d0}/  "));
        FormatCountTextBuffer(
            &accountText, format, accountIcon, formattingMode);
    }

    if (connectionIcon != -1) {
        const int formattingMode = g_NumberFormattingMode;
        PlayerIconStringArgument format;
        FormatterSetPattern(
            &format, nullptr, CSF_STR(0x00841B28, "@@{d0}/"));
        FormatCountTextBuffer(
            &connectionText, format, connectionIcon, formattingMode);
    }

    const std::uint16_t* accountData =
        CSF_AT(std::uint16_t*, &accountText, 4);
    if (CSF_AT(std::uint32_t, &accountText, 24) < 8)
        accountData = reinterpret_cast<const std::uint16_t*>(
            accountText.bytes + 4);
    AppendWideString(output, accountData);

    const std::uint16_t* connectionData =
        CSF_AT(std::uint16_t*, &connectionText, 4);
    if (CSF_AT(std::uint32_t, &connectionText, 24) < 8)
        connectionData = reinterpret_cast<const std::uint16_t*>(
            connectionText.bytes + 4);
    AppendWideString(output, connectionData);

    if (CSF_AT(std::uint32_t, &connectionText, 24) >= 8 &&
        CSF_AT(void*, &connectionText, 4)) {
        Mem_FreeDebug(CSF_AT(void*, &connectionText, 4));
    }

    const std::uint32_t accountCapacity =
        CSF_AT(std::uint32_t, &accountText, 24);
    CSF_AT(std::uint32_t, &connectionText, 24) = 7;
    CSF_AT(std::uint32_t, &connectionText, 20) = 0;
    CSF_AT(std::uint16_t, &connectionText, 4) = 0;

    if (accountCapacity >= 8 && CSF_AT(void*, &accountText, 4))
        Mem_FreeDebug(CSF_AT(void*, &accountText, 4));

    return flags;
}

HOOK(0x0055E110, CMenuPlayerList_BuildPlayerStatusIcons, "auto");
