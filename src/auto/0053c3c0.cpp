#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ShowSaveGameMessage, 0x0050F860, (void* self, void* , int text, int messageAction, int configuration));
CSF_FUNC(int, __fastcall, C_00840af8_FindEntryIndexByName, 0x00515FC0, (C_00840af8* self, void* , unsigned int* nameQuery));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreParser_FormatScenarioText, 0x00692660, (int outputString, int descriptionText, int count, int flag));

static char __fastcall CMenuProfilesPC_CheckProfileNameConflict(
    CMenuProfilesPC* self, void* /*edx*/, unsigned int* nameQuery)
{
    if (C_00840af8_FindEntryIndexByName(
            reinterpret_cast<C_00840af8*>(self->field_0028),
            nullptr, nameQuery) == -1)
    {
        self->field_0038 = -1;
        return 0;
    }

    alignas(8) uint8_t menuBuffer[0xB4];
    // Includes the four-byte string header and its adjacent 0x18-byte storage.
    alignas(8) uint8_t formattedText[0x1C];

    MenuBuffer_Init(menuBuffer, nullptr);
    const unsigned int nameCapacity = nameQuery[6];

    CSF_AT(int, menuBuffer, 0x08) = 2;
    CSF_AT(char, menuBuffer, 0x0C) = 0;
    CSF_AT(unsigned int, formattedText, 0x18) = 7;
    CSF_AT(int, formattedText, 0x14) = 0;
    CSF_AT(std::uint16_t, formattedText, 0x04) = 0;

    const void* nameText;
    if (nameCapacity < 8)
        nameText = reinterpret_cast<uint8_t*>(nameQuery) + 4;
    else
        nameText = reinterpret_cast<void*>(nameQuery[1]);

    CoreParser_FormatScenarioText(
        reinterpret_cast<int>(formattedText),
        reinterpret_cast<int>(CSF_STR(0x008429AC, "pr15")),
        reinterpret_cast<int>(nameText), 1);

    void* messageText;
    if (CSF_AT(unsigned int, formattedText, 0x18) < 8)
        messageText = formattedText + 4;
    else
        messageText = CSF_AT(void*, formattedText, 0x04);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ShowSaveGameMessage(
        globalState, nullptr, reinterpret_cast<int>(messageText), 3, 0);

    const bool usesHeapStorage =
        CSF_AT(unsigned int, formattedText, 0x18) >= 8;
    self->field_0038 = 1;
    if (usesHeapStorage)
    {
        void* block = CSF_AT(void*, formattedText, 0x04);
        if (block)
            Mem_FreeDebug(block);
    }

    CSF_AT(unsigned int, formattedText, 0x18) = 7;
    CSF_AT(int, formattedText, 0x14) = 0;
    CSF_AT(std::uint16_t, formattedText, 0x04) = 0;
    SceneLoader_DestroyMenuBuffer(
        reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
    return 1;
}

HOOK(0x0053C3C0, CMenuProfilesPC_CheckProfileNameConflict, "auto");
