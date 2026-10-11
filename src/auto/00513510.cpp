#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetModelSummary, 0x00501EE0, (void* self, void* , C_0082d410* output));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, CSaveGameManager_FinishSave, 0x00511FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSaveGameManager_BeginSave, 0x005120A0, (void* self, void* edx, int a2, int a3, char a4));
CSF_FUNC(char*, __fastcall, CSaveGameManager_ResolveSavePath, 0x00513250, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, ProfilesSavegames_GetSaveStateId, 0x00515C20, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(char, __fastcall, C_0085e748_Finalize, 0x00699FD0, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall CSaveGameManager_SaveGame(
    CSaveGameManager* self, void* /*edx*/, uint32_t* saveData)
{
    alignas(8) uint8_t context[0x28]{};

    CSF_AT(char, context, 0x24) = GetOptionsByteAt2();
    if (CSF_AT(char, context, 0x24))
        SceneLoader_SetFileOption(0);

    auto* saveName = reinterpret_cast<C_0082d410*>(
        reinterpret_cast<uint8_t*>(saveData) + 0x50);

    if (!CompareStringSubstring(
            saveName, nullptr, 0, saveData[25],
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        CSaveGameManager_ResolveSavePath(self, nullptr, saveName);
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    CSF_AT(uint32_t, context, 0x04) = 0;
    CSF_AT(uint32_t, context, 0x08) = 0;
    CSF_AT(uint32_t, context, 0x0C) = 0;
    CSF_AT(uint8_t, context, 0x14) = 0;
    CSF_AT(uint8_t, context, 0x15) = 0;
    CSF_AT(int, context, 0x18) = 100;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    CSF_AT(uint32_t*, context, 0x20) = nullptr;

    if (!CSaveGameManager_BeginSave(
            self, nullptr, reinterpret_cast<int>(context),
            reinterpret_cast<int>(saveName), 1))
    {
        SceneLoader_SetFileOption(CSF_AT(char, context, 0x24));
        return 0;
    }

    void* game = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    saveData[13] = ProfilesSavegames_GetSaveStateId(
        CSF_AT(void*, game, 0x08), nullptr);

    game = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* stateOwner = CSF_AT(void*, game, 0x08);
    void* state = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, stateOwner, 0)[0x24 / 4])(stateOwner, nullptr);

    auto* summary = reinterpret_cast<uint8_t*>(state) + 0x1C;
    saveData[14] = CSF_AT(uint32_t, summary, 0x00);
    saveData[15] = CSF_AT(uint32_t, summary, 0x04);
    saveData[16] = CSF_AT(uint32_t, summary, 0x08);
    saveData[17] = CSF_AT(uint32_t, summary, 0x0C);
    saveData[18] = CSF_AT(uint32_t, summary, 0x10);
    saveData[19] = CSF_AT(uint32_t, summary, 0x14);

    void* descriptionOwner = reinterpret_cast<void*>(GetValue8B0554());
    const uint32_t capacity = CSF_AT(uint32_t, descriptionOwner, 0x1AD0);
    auto* descriptionString =
        reinterpret_cast<uint8_t*>(descriptionOwner) + 0x1AB8;
    const char* description = capacity < 0x10
        ? reinterpret_cast<const char*>(descriptionString + 4)
        : CSF_AT(const char*, descriptionString, 4);

    saveData[43] = MenuExtras_ParseDescriptionId(description);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    SceneLoader_GetModelSummary(
        sceneLoader, nullptr,
        reinterpret_cast<C_0082d410*>(
            reinterpret_cast<uint8_t*>(saveData) + 0x6C));

    CSF_AT(uint32_t*, context, 0x20) = saveData;
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x40 / 4])(self, nullptr, context);

    if (!C_0085e748_Finalize(
            reinterpret_cast<void*>(self->field_0024), nullptr))
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
    }

    void* saveStream = CSF_AT(void*, context, 0x0C);
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, saveStream, 0)[0x10 / 4])(saveStream, nullptr);

    self->field_0018 = 1;
    CSaveGameManager_FinishSave(self, nullptr);
    SceneLoader_SetFileOption(CSF_AT(char, context, 0x24));

    return CSF_AT(uint8_t, context, 0x00) == 0;
}

HOOK(0x00513510, CSaveGameManager_SaveGame, "auto");
