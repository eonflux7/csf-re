#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetCharacterSettings, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(uint8_t, __fastcall, MenuMpPause_QueryGlobalFlagIsOne, 0x0073E040, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static char __fastcall MpSpawnPoints_UpdateCharacterSelection(
    void* self, void* /*edx*/, int /*unused*/)
{
    if (!GetGlobalState_6A6D70())
        return 0;

    void* state = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_6A6D70()));
    if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, state, 0)[0x74 / 4])(state, nullptr))
        return 0;

    const uint32_t selectionAddress = g_CharacterSelectState8B0694;
    if (!selectionAddress)
        return 0;

    void* selection = reinterpret_cast<void*>(
        static_cast<uintptr_t>(selectionAddress));
    const uint32_t selectedSubobject = CSF_AT(uint32_t, selection, 4);
    if (!selectedSubobject)
        return static_cast<char>(selectionAddress);

    const uint32_t characterAddress = selectedSubobject - 776;
    if (!characterAddress)
        return static_cast<char>(selectionAddress);

    void* character = reinterpret_cast<void*>(
        static_cast<uintptr_t>(characterAddress));
    void* characterSubobject = static_cast<uint8_t*>(character) + 0x308;
    if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, characterSubobject, 0)[8 / 4])(
                  characterSubobject, nullptr))
        return 0;

    const int previousFlag = CSF_AT(int, self, 0x145C);
    void* multiplayerState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalDword_73C020()));
    const int globalTeam = CSF_AT(int, multiplayerState, 0x100);

    char* settings = MpSpawnPoints_GetCharacterSettings(character, nullptr);
    const int characterTeam = CSF_AT(int8_t, settings, 5);

    multiplayerState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalDword_73C020()));
    const int currentFlag = static_cast<int8_t>(
        MenuMpPause_QueryGlobalFlagIsOne(multiplayerState, nullptr));
    CSF_AT(int, self, 0x145C) = currentFlag;

    if (currentFlag == previousFlag &&
        globalTeam == characterTeam &&
        CSF_AT(uint8_t, self, 0x1460) == 0)
        return 0;

    const int updateBlocker = CSF_AT(int, self, 0x1444);
    if (updateBlocker)
        return static_cast<char>(updateBlocker);

    CSF_AT(uint8_t, self, 0x1460) = 0;
    state = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalState_6A6D70()));
    settings = MpSpawnPoints_GetCharacterSettings(character, nullptr);
    const bool characterFlag = CSF_AT(uint8_t, settings, 0x107) != 0;

    return ((char(__fastcall*)(void*, void*, int, bool, int))
                CSF_AT(void**, state, 0)[0x78 / 4])(
                    state, nullptr, CSF_AT(int, self, 0x145C),
                    characterFlag, characterTeam);
}

HOOK(0x00551340, MpSpawnPoints_UpdateCharacterSelection, "auto");
