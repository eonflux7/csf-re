#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(char*, __fastcall, C_00860288_GetCharacterData, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int16_t*, __fastcall, CharacterSelect_SetWideName, 0x0073D050, (void* self, void* edx, int16_t* a2));
CSF_FUNC(void*, __fastcall, C_00867768_ConstructNameNode, 0x0073F700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharacterSelect_SetSourceObject, 0x0073F730, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_GLOBAL(uint32_t, g_MenuMPCharacterSelect_LastState, 0x008B0698);

static uint32_t __cdecl MpSpawnPoints_InitializeCharacterSelection(int sourceObject)
{
    C_00860288* character = sourceObject
        ? reinterpret_cast<C_00860288*>(
            static_cast<uint32_t>(sourceObject) - 776)
        : nullptr;

    void* storage = Mem_Alloc(8);
    void* nameNode = storage
        ? C_00867768_ConstructNameNode(storage, nullptr)
        : nullptr;

    g_CharacterSelectState8B0694 = reinterpret_cast<uint32_t>(nameNode);
    CharacterSelect_SetSourceObject(nameNode, nullptr, sourceObject);

    char* characterData = C_00860288_GetCharacterData(character, nullptr);
    int selection = CSF_AT(int8_t, characterData, 5);
    void* selectState = reinterpret_cast<void*>(GetGlobalDword_73C020());
    CSF_AT(int, selectState, 0x100) = selection;

    characterData = C_00860288_GetCharacterData(character, nullptr);
    uint16_t wideName[130];
    CopyTrucoStringToWide(characterData + 263, wideName, 128);

    selectState = reinterpret_cast<void*>(GetGlobalDword_73C020());
    CharacterSelect_SetWideName(
        selectState, nullptr, reinterpret_cast<int16_t*>(wideName));

    uint32_t lastState = CSF_AT(uint32_t, character, 0x2B8);
    g_MenuMPCharacterSelect_LastState = lastState;
    return lastState;
}

HOOK(0x00551AD0, MpSpawnPoints_InitializeCharacterSelection, "auto");
