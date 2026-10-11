#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_GetObjectIdentifier, 0x004A1980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, SpawnPoints_SendEvent, 0x00551680, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, SpawnPoints_GetCharacterUpdateValue, 0x0073CD30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SpawnPoints_UpdateCharacter, 0x00741150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SpawnPoints_UpdateMultiplayerStats, 0x00747FB0, (void* self, void* edx, char a2, char a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static int __fastcall C_00844694_ProcessCharacterEvent(
    C_00844694* self, void* /*edx*/, int eventAddress)
{
    void* event = reinterpret_cast<void*>(eventAddress);
    int result = CSF_AT(int, event, 0x10);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(event) + 8));
        if (result)
        {
            uint32_t characterPart = CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x18);
            void* otherCharacter = nullptr;
            if (characterPart)
            {
                otherCharacter = reinterpret_cast<void*>(characterPart - 1712);
                if (otherCharacter)
                {
                    int context = GetGlobalDword_73C020();
                    int updateValue = SpawnPoints_GetCharacterUpdateValue(
                        reinterpret_cast<void*>(context), nullptr);
                    SpawnPoints_UpdateCharacter(otherCharacter, nullptr, updateValue);
                }
            }

            result = static_cast<int>(CSF_AT(uint32_t, event, 0x2C) - 1);
            if (result != 0)
                return result;

            void* stats = nullptr;
            uint32_t selection = g_CharacterSelectState8B0694;
            if (selection)
            {
                uint32_t statsPart = CSF_AT(uint32_t, reinterpret_cast<void*>(selection), 0x4);
                if (statsPart)
                    stats = reinterpret_cast<void*>(statsPart - 776);
            }

            result = static_cast<int>(self->field_0c8c);
            void* character = nullptr;
            if (result)
            {
                result = CSF_AT(int, reinterpret_cast<void*>(result), 0x18);
                if (result)
                    character = reinterpret_cast<void*>(static_cast<uint32_t>(result) - 1712);
            }
            if (!stats || !character)
                return result;

            SpawnPoints_UpdateMultiplayerStats(
                stats, nullptr,
                static_cast<char>(CSF_AT(uint8_t, character, 0x443)),
                static_cast<char>(CSF_AT(uint8_t, event, 0x24)));

            if (CSF_AT(int, event, 0x24) == 1)
            {
                uint32_t otherBuffer = 0;
                int otherId = static_cast<int>(*SpawnPoints_GetObjectIdentifier(
                    otherCharacter, nullptr, &otherBuffer));
                uint32_t characterBuffer = 0;
                int characterId = static_cast<int>(*SpawnPoints_GetObjectIdentifier(
                    character, nullptr, &characterBuffer));
                return SpawnPoints_SendEvent(
                    self, nullptr, 1396846640, characterId, otherId, -1);
            }

            uint32_t otherBuffer = 0;
            int otherId = static_cast<int>(*SpawnPoints_GetObjectIdentifier(
                otherCharacter, nullptr, &otherBuffer));
            uint32_t characterBuffer[2] = {};
            uint32_t* identifier = SpawnPoints_GetObjectIdentifier(
                character, nullptr, characterBuffer);
            int eventValue = CSF_AT(int, event, 0x24);
            int characterId = static_cast<int>(*identifier);
            return SpawnPoints_SendEvent(
                self, nullptr, 1396846641, characterId, otherId, eventValue);
        }
    }

    if (CSF_AT(int, event, 0x2C) == 2)
    {
        void* stats = nullptr;
        uint32_t selection = g_CharacterSelectState8B0694;
        if (selection)
        {
            uint32_t statsPart = CSF_AT(uint32_t, reinterpret_cast<void*>(selection), 0x4);
            if (statsPart)
                stats = reinterpret_cast<void*>(statsPart - 776);
        }

        result = static_cast<int>(self->field_0c8c);
        void* character = nullptr;
        if (result)
        {
            result = CSF_AT(int, reinterpret_cast<void*>(result), 0x18);
            if (result)
                character = reinterpret_cast<void*>(static_cast<uint32_t>(result) - 1712);
        }
        if (!stats || !character)
            return result;

        SpawnPoints_UpdateMultiplayerStats(
            stats, nullptr,
            static_cast<char>(CSF_AT(uint8_t, character, 0x443)),
            static_cast<char>(CSF_AT(uint8_t, event, 0x24)));

        if (CSF_AT(int, event, 0x24) == 1)
        {
            uint32_t buffer = 0;
            int characterId = static_cast<int>(*SpawnPoints_GetObjectIdentifier(
                character, nullptr, &buffer));
            return SpawnPoints_SendEvent(
                self, nullptr, 1396846648, characterId, -1, -1);
        }

        uint32_t buffer = 0;
        uint32_t* identifier = SpawnPoints_GetObjectIdentifier(character, nullptr, &buffer);
        int eventValue = CSF_AT(int, event, 0x24);
        int characterId = static_cast<int>(*identifier);
        return SpawnPoints_SendEvent(
            self, nullptr, 1396846649, characterId, -1, eventValue);
    }
    return result;
}

HOOK(0x005522D0, C_00844694_ProcessCharacterEvent, "auto");
