#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(bool, __fastcall, NetProfile_CheckAccountName, 0x005184B0, (void* self, void* , const char* accountName, char expectedLeadByte));
CSF_FUNC(char, __cdecl, MenuAccount_IsNameAllSpaces, 0x0055C900, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_FormatScenarioText, 0x00692660, (int outputString, int descriptionText, int count, int flag));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static int __fastcall S_0055c780_FallbackToPlayerList(
    S_0055c780* self, void* /*edx*/)
{
    alignas(8) uint8_t accountTextBytes[0x44];
    alignas(8) uint8_t patternBytes[0x1C];
    alignas(8) uint8_t localizedBytes[0x1C];
    auto& localized = *reinterpret_cast<S_00696af0*>(localizedBytes);

    uint8_t* accountName = &self->field_003c;
    S_004f21d0_SetPattern(
        reinterpret_cast<S_004f21d0*>(patternBytes), nullptr, accountName);

    int result;
    if (g_MpNetworkType == 2)
    {
        wchar_t* nameText =
            reinterpret_cast<wchar_t*>(CSF_AT(uint32_t, patternBytes, 0x04));
        if (CSF_AT(uint32_t, patternBytes, 0x18) < 8)
            nameText = reinterpret_cast<wchar_t*>(patternBytes + 0x04);

        ConstructWideString(localizedBytes, nullptr, nameText);
        char allSpaces = MenuAccount_IsNameAllSpaces(
            reinterpret_cast<uint32_t*>(localizedBytes));

        if (localized.field_0018 >= 8 && localized.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(localized.field_0004));

        if (allSpaces)
        {
            void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
            result = GameBddLoader_DispatchMenuRequest(
                loader, nullptr, 1886531892, 1, 1, 0);
            self->field_0280 = 3;
        }
        else
        {
            void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8B056C());
            void* profile = CSF_AT(void*, globalValue, 0x14);
            if (NetProfile_CheckAccountName(
                    profile, nullptr, reinterpret_cast<const char*>(accountName), 1))
            {
                localized.field_0018 = 7;
                CSF_AT(uint32_t, localizedBytes, 0x14) = 0;
                CSF_AT(uint16_t, localizedBytes, 0x04) = 0;

                CopyTrucoStringToWide(
                    reinterpret_cast<const char*>(accountName),
                    reinterpret_cast<uint16_t*>(accountTextBytes), 33);
                CoreParser_FormatScenarioText(
                    reinterpret_cast<int>(localizedBytes),
                    reinterpret_cast<int>(CSF_STR(0x008454F8, "ae24")),
                    reinterpret_cast<int>(accountTextBytes), 1);

                uint32_t messageText = localized.field_0004;
                if (localized.field_0018 < 8)
                    messageText = reinterpret_cast<uint32_t>(&localized.field_0004);

                void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
                GameBddLoader_ShowSaveGameMessage(
                    loader, nullptr, static_cast<int>(messageText), 1, 0);
                self->field_0280 = 3;
                result = SaveGame_DestroyLocalizedString(localizedBytes, nullptr);
            }
            else
            {
                result = reinterpret_cast<int (__fastcall*)(S_0055c780*, void*)>(
                    reinterpret_cast<void**>(self->field_0000)[0x58 / 4])(
                        self, nullptr);
            }
        }
    }
    else
    {
        result = reinterpret_cast<int (__fastcall*)(S_0055c780*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x58 / 4])(
                self, nullptr);
    }

    if (CSF_AT(uint32_t, patternBytes, 0x18) >= 8)
    {
        void* allocation =
            reinterpret_cast<void*>(CSF_AT(uint32_t, patternBytes, 0x04));
        result = reinterpret_cast<int>(allocation);
        if (allocation)
            return Mem_FreeDebug(allocation);
    }
    return result;
}

HOOK(0x0055C9D0, S_0055c780_FallbackToPlayerList, "auto");
