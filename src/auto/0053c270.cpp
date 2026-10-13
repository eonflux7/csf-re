#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_FormatScenarioText, 0x00692660, (int outputString, int descriptionText, int count, int flag));

static std::uint32_t __fastcall CMenuProfilesPC_ShowProfileConfirmation(
    CMenuProfilesPC* self, void* /*edx*/)
{
    void* profileList = reinterpret_cast<void*>(self->field_0028);
    std::uint32_t selectedIndex = self->field_0030;
    self->field_0038 = 3;

    const bool alternatePrompt =
        reinterpret_cast<bool(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, profileList, 0)[0x2C / 4])(
                profileList, nullptr, selectedIndex);

    alignas(8) uint8_t configurationBytes[0xB4];
    // The formatter's string object includes its inline storage and metadata.
    alignas(8) uint8_t messageBytes[0x1C];

    MenuBuffer_Init(configurationBytes, nullptr);
    profileList = reinterpret_cast<void*>(self->field_0028);
    selectedIndex = self->field_0030;

    CSF_AT(std::uint32_t, configurationBytes, 0x08) = 2;
    CSF_AT(uint8_t, configurationBytes, 0x0C) = 0;
    CSF_AT(std::uint32_t, messageBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, messageBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, messageBytes, 0x04) = 0;

    void* entry =
        reinterpret_cast<void*(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, profileList, 0)[0x1C / 4])(
                profileList, nullptr, selectedIndex);

    const char* profileName;
    if (CSF_AT(std::uint32_t, entry, 0x18) < 8u)
        profileName = reinterpret_cast<const char*>(entry) + 4;
    else
        profileName = CSF_AT(const char*, entry, 0x04);

    CoreParser_FormatScenarioText(
        reinterpret_cast<int>(messageBytes),
        reinterpret_cast<int>(alternatePrompt
            ? CSF_STR(0x008429A4, "pr12")
            : CSF_STR(0x0084299C, "pro6")),
        reinterpret_cast<int>(profileName),
        1);

    const char* messageText;
    if (CSF_AT(std::uint32_t, messageBytes, 0x18) < 8u)
        messageText = reinterpret_cast<const char*>(messageBytes + 0x04);
    else
        messageText = CSF_AT(const char*, messageBytes, 0x04);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ShowSaveGameMessage(
        globalState, nullptr, reinterpret_cast<int>(messageText), 3,
        reinterpret_cast<int>(configurationBytes));

    if (CSF_AT(std::uint32_t, messageBytes, 0x18) >= 8u &&
        CSF_AT(void*, messageBytes, 0x04))
    {
        Mem_FreeDebug(CSF_AT(void*, messageBytes, 0x04));
    }

    CSF_AT(std::uint16_t, messageBytes, 0x04) = 0;
    CSF_AT(std::uint32_t, messageBytes, 0x14) = 0;
    CSF_AT(std::uint32_t, messageBytes, 0x18) = 7;

    return SceneLoader_DestroyMenuBuffer(
        reinterpret_cast<std::uint32_t*>(configurationBytes), nullptr);
}

HOOK(0x0053C270, CMenuProfilesPC_ShowProfileConfirmation, "auto");
