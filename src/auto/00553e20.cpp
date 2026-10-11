#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeSelection, 0x00550C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MpSpawnPoints_DestroySelection, 0x00552F60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(C_008310b8*, __stdcall, MpSpawnPoints_SetSelectionParameter, 0x0073C7C0, (int a1));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ApplySelection, 0x00747F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshSelection, 0x007480A0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetSecondaryString, 0x007496C0, (void* self, void* edx, char* Source));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetPrimaryString, 0x007496E0, (void* self, void* edx, char* Source));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_SetTertiaryString, 0x00749700, (void* self, void* edx, char* Source));

static void __fastcall C_00844694_ApplyCharacterSelection(
    C_00844694* self, void* /*edx*/,
    char selectionType, char* primaryString, char option4, char option5,
    char selectionMode, int /*unused*/, char* tertiaryString,
    int selectionParameter, char* secondaryString,
    char option11, char option12, char option13,
    int16_t shortValue, int value)
{
    if (self->field_14a2)
        return;
    if (!g_CharacterSelectState8B0694)
        return;

    const uint32_t selectedObject =
        CSF_AT(uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
    if (!selectedObject)
        return;

    auto* character = reinterpret_cast<C_008310b8*>(selectedObject - 0x308);
    if (!character)
        return;

    alignas(4) uint8_t selection[0x254]{};
    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(selection), nullptr);
    CSF_AT(uint8_t, selection, 0x19E) = 0;
    CSF_AT(int, selection, 0x1B8) = 15;
    CSF_AT(int, selection, 0x1B4) = 0;
    CSF_AT(uint8_t, selection, 0x1A4) = 0;
    MpSpawnPoints_InitializeSelection(selection, nullptr);

    MpSpawnPoints_SetPrimaryString(selection, nullptr, primaryString);
    MpSpawnPoints_SetSecondaryString(selection, nullptr, secondaryString);
    CSF_AT(char, selection, 0x6) = option4;
    CSF_AT(char, selection, 0x5) = option5;
    MpSpawnPoints_SetTertiaryString(selection, nullptr, tertiaryString);

    CSF_AT(char, selection, 0x0) = option11;
    CSF_AT(char, selection, 0x1) = option12;
    CSF_AT(int16_t, selection, 0x196) = shortValue;
    CSF_AT(int16_t, selection, 0x198) = shortValue;
    CSF_AT(int16_t, selection, 0x19A) = shortValue;
    CSF_AT(int16_t, selection, 0x19C) = shortValue;
    CSF_AT(char, selection, 0x4) = selectionType;
    CSF_AT(int, selection, 0x188) = value;
    CSF_AT(int, selection, 0x190) = value;
    CSF_AT(int16_t, selection, 0x194) = static_cast<int16_t>(value);
    CSF_AT(int, selection, 0x18C) = value;
    CSF_AT(char, selection, 0x2) = option13;
    CSF_AT(char, selection, 0x3) = selectionMode;

    MpSpawnPoints_ApplySelection(character, nullptr, reinterpret_cast<int>(selection));
    MpSpawnPoints_RefreshSelection(character, nullptr);
    GetGlobalDword_73C020();
    MpSpawnPoints_SetSelectionParameter(selectionParameter);
    CSF_AT(char, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x114) =
        selectionMode;

    const uint32_t nextRevision = CSF_AT(uint32_t, character, 0x2B8) + 1;
    if (nextRevision != CSF_AT(uint32_t, character, 0x2B8))
        CSF_AT(uint32_t, character, 0x2B8) = nextRevision;

    MpSpawnPoints_DestroySelection(selection, nullptr);
}

HOOK(0x00553E20, C_00844694_ApplyCharacterSelection, "auto");
