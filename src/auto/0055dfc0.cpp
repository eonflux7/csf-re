#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00858180_GetObjectId, 0x004A1980, (C_00858180* self, void* , uint32_t* outId));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __cdecl, VariantValues_AppendWideString, 0x00695700, (uint16_t* dest, const uint16_t* src));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* a1, char a2, void* Block, int a4, int a5, int a6, int a7, std::uint32_t a8, char arg20));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(char, __cdecl, NetCallbacks_IsAlternatePlayerState, 0x00743920, (int a1));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static int __stdcall MenuMpAccounts_FormatPlayerStatus(
    int playerAddress, uint16_t* destination)
{
    if (!destination)
        return 0;

    *destination = 0;
    int statusText = -1;
    void* player = reinterpret_cast<void*>(playerAddress);
    void* manager = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobal8B0688()), 0x1484);

    uint32_t playerId = CSF_AT(uint32_t, player, 0x2C);
    if ((playerId & 0x3FFFFF) == 0) {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        playerId = 0;
    }

    int statusFlags;
    if ((reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, manager, 0)[0x18 / 4]))(
            manager, nullptr, playerId)) {
        statusText = 26;
        statusFlags = 16;
    } else if ((reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
                   CSF_AT(void**, manager, 0)[0x20 / 4]))(
                   manager, nullptr, CSF_AT(uint32_t, player, 0x674))) {
        uint32_t objectId;
        uint32_t* id = C_00858180_GetObjectId(
            reinterpret_cast<C_00858180*>(player), nullptr, &objectId);
        char alternate = NetCallbacks_IsAlternatePlayerState(
            static_cast<int>(*id));
        statusFlags = 8;
        statusText = alternate ? 30 : 25;
    } else {
        statusFlags = 32;
        if (!(reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, manager, 0)[0x28 / 4]))(
                manager, nullptr, CSF_AT(uint32_t, player, 0x674))
            && !(reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, manager, 0)[0x24 / 4]))(
                manager, nullptr, CSF_AT(uint32_t, player, 0x674))) {
            statusText = 29;
        }
    }

    // The frame holds one complete 28-byte string object.
    alignas(8) uint8_t textBytes[0x1C];
    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(textBytes), nullptr, &g_byte_82E65C);

    if (statusText != -1) {
        int formattingMode = g_NumberFormattingMode;
        alignas(8) uint8_t patternBytes[0x1C] = {};
        S_004f21d0_SetPattern(
            reinterpret_cast<S_004f21d0*>(patternBytes), nullptr,
            const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(
                CSF_STR(0x00841B28, "@@{d0}/"))));

        // The original supplies the formatting mode after the declared
        // arguments; the formatter consumes that trailing stack argument.
        using FormatWithMode = void* (__cdecl*)(
            void*, char, void*, int, int, int, int, std::uint32_t, char, int);
        reinterpret_cast<FormatWithMode>(FormatCountTextBuffer)(
            textBytes,
            CSF_AT(char, patternBytes, 0),
            CSF_AT(void*, patternBytes, 4),
            CSF_AT(int, patternBytes, 8),
            CSF_AT(int, patternBytes, 0xC),
            CSF_AT(int, patternBytes, 0x10),
            CSF_AT(int, patternBytes, 0x14),
            CSF_AT(std::uint32_t, patternBytes, 0x18),
            static_cast<char>(statusText),
            formattingMode);
    }

    const uint16_t* text = CSF_AT(const uint16_t*, textBytes, 4);
    if (CSF_AT(std::uint32_t, textBytes, 0x18) < 8)
        text = reinterpret_cast<const uint16_t*>(textBytes + 4);
    VariantValues_AppendWideString(destination, text);

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8) {
        void* block = CSF_AT(void*, textBytes, 4);
        if (block)
            Mem_FreeDebug(block);
    }

    return statusFlags;
}

HOOK(0x0055DFC0, MenuMpAccounts_FormatPlayerStatus, "auto");
