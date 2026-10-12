#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MpGameModes_BuildRulesBlock, 0x0054F680, (void* self, void* edx, uint32_t* a1));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetValue_8B08F8, 0x0055A9C0, ());
CSF_FUNC(int, __fastcall, CopyRulesBlockAndApply, 0x0055A9F0, (int* self, void* , const void* sourceBlock));
CSF_FUNC(int, __fastcall, MpRules_CopyBlockAndApply, 0x0055AA20, (int* self, void* , const void* sourceBlock));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_ConfigurePrudpEndpoints_73C530, 0x0073C530, (int secondaryOption, int secondaryAttribute, int secondaryValue, int primaryOption, int primaryAttribute, int primaryValue));
CSF_FUNC(int, __cdecl, NetSession_ConfigurePrudpEndpoints_73C5E0, 0x0073C5E0, ());
CSF_FUNC(int, __fastcall, MpGameModes_GetConnectionSetting, 0x00744480, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_749b20_TakeByteFlag, 0x00749B20, (void* self, void*));

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void* S_0054ecf0_GetConnectionSettings(S_0054ecf0* self)
{
    const std::uint32_t owner = self->field_0004
        ? self->field_0004 - 216
        : 0;
    return reinterpret_cast<void*>(owner + 168);
}

static char __fastcall S_0054ecf0_ApplyPendingRules(
    S_0054ecf0* self, void* /*edx*/)
{
    const std::uint32_t interfaceAddress = self->field_0004;
    if (!interfaceAddress || interfaceAddress == 216)
        return static_cast<char>(interfaceAddress);

    void* interfaceObject = reinterpret_cast<void*>(interfaceAddress);
    if (!((bool (__fastcall*)(void*, void*))
          CSF_AT(void**, interfaceObject, 0)[8 / 4])(interfaceObject, nullptr))
    {
        self->field_1099 |= C_749b20_TakeByteFlag(
            S_0054ecf0_GetConnectionSettings(self), nullptr);
    }

    const char pending = static_cast<char>(self->field_1099);
    if (!pending)
        return pending;

    if (MpGameModes_GetConnectionSetting(
            S_0054ecf0_GetConnectionSettings(self), nullptr, 11))
    {
        int secondaryValue = 57958;
        int primaryValue = 57958;

        switch (MpGameModes_GetConnectionSetting(
                    S_0054ecf0_GetConnectionSettings(self), nullptr, 9))
        {
        case 0:
            secondaryValue = 57958;
            break;
        case 1:
            secondaryValue = 0x20000;
            break;
        case 2:
            secondaryValue = 0x40000;
            break;
        case 3:
            secondaryValue = static_cast<int>(
                static_cast<std::uint32_t>(MpGameModes_GetConnectionSetting(
                    S_0054ecf0_GetConnectionSettings(self), nullptr, 7)) << 10);
            break;
        default:
            break;
        }

        switch (MpGameModes_GetConnectionSetting(
                    S_0054ecf0_GetConnectionSettings(self), nullptr, 10))
        {
        case 1:
            primaryValue = 0x20000;
            break;
        case 2:
            primaryValue = 0x40000;
            break;
        case 3:
            primaryValue = static_cast<int>(
                static_cast<std::uint32_t>(MpGameModes_GetConnectionSetting(
                    S_0054ecf0_GetConnectionSettings(self), nullptr, 8)) << 10);
            break;
        default:
            break;
        }

        // These four queries use the same settings object, captured before
        // the first query rather than reloaded between calls.
        void* settings = S_0054ecf0_GetConnectionSettings(self);
        const int primaryAttribute =
            MpGameModes_GetConnectionSetting(settings, nullptr, 6);
        const int primaryOption =
            MpGameModes_GetConnectionSetting(settings, nullptr, 5);
        const int secondaryAttribute =
            MpGameModes_GetConnectionSetting(settings, nullptr, 4);
        const int secondaryOption =
            MpGameModes_GetConnectionSetting(settings, nullptr, 3);

        GetGlobalDword_73C020();
        NetSession_ConfigurePrudpEndpoints_73C530(
            secondaryOption, secondaryAttribute, secondaryValue,
            primaryOption, primaryAttribute, primaryValue);
    }
    else
    {
        GetGlobalDword_73C020();
        NetSession_ConfigurePrudpEndpoints_73C5E0();
    }

    alignas(8) std::uint8_t rulesBytes[0xA0];
    MpGameModes_BuildRulesBlock(
        self, nullptr, reinterpret_cast<std::uint32_t*>(rulesBytes));

    CoreFiles_SetFileOption(0);
    GetGlobal8B0688();

    if (g_OnlineMatchFlag)
    {
        int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
        MpRules_CopyBlockAndApply(rules, nullptr, rulesBytes);
    }
    else
    {
        int* rules = reinterpret_cast<int*>(GetValue_8B08F8());
        CopyRulesBlockAndApply(rules, nullptr, rulesBytes);
    }

    void* result = CoreFiles_SetFileOption(1);
    self->field_1099 = 0;
    return static_cast<char>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x005505F0, S_0054ecf0_ApplyPendingRules, "auto");
