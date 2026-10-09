#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSceneService, 0x00442A00, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0044f670_ClearByte128, 0x0044F670, (void* self, void*));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_PrepareSceneIndex, 0x00450630, (int a1, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_ResetCurrentScene, 0x00450770, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_AddSceneEntry, 0x00453150, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareCurrentScene, 0x00455240, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __stdcall, SceneLoader_RegisterReferencedObject, 0x004FF2F0, (int a1, char a2));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneIndex, 0x004FF9B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_FindEntryRecord, 0x00500410, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_UpdateSceneEntries, 0x00500800, (void* self, void* edx));
CSF_FUNC(int, __stdcall, SceneLoader_RegisterSceneContext, 0x00501360, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_CompleteSceneActivation, 0x00503EF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneContextProvider, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetContextObject, 0x006B6AF0, (void* self, void* edx));

static int __fastcall CSceneLoader_ActivateScene(
    C_0082d410* self, void* /*edx*/, int sceneIndex,
    C_00834050* focusEntry, int firstEntry, int secondEntry,
    int* focusConfiguration, int focusMode)
{
    SceneLoader_PrepareSceneIndex(sceneIndex, 0);
    SceneLoader_ResetSceneService(
        reinterpret_cast<void*>(GetValue_8afc80()), nullptr);
    SceneLoader_PrepareCurrentScene(g_pCurrentScene, nullptr);
    SceneLoader_ResetCurrentScene(g_pCurrentScene, nullptr);
    C_0044f670_ClearByte128(g_pCurrentScene, nullptr);

    void* provider = reinterpret_cast<void*>(
        SceneLoader_GetSceneContextProvider(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* context =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x44 / 4])(provider, nullptr);
    SceneLoader_RegisterSceneContext(reinterpret_cast<int>(context));

    context = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, provider, 0)[0x44 / 4])(provider, nullptr);
    int contextObject = SceneLoader_GetContextObject(context, nullptr);
    SceneLoader_RegisterReferencedObject(contextObject, 0);

    void* reference =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x40 / 4])(provider, nullptr);
    void* referencedObject = GetReferencedObject(reference, nullptr);
    SceneLoader_RegisterReferencedObject(
        reinterpret_cast<int>(referencedObject), 1);

    if (firstEntry)
    {
        void* record = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(self, nullptr, sceneIndex, firstEntry));
        if (CSF_AT(int, record, 8) >= 0)
        {
            CSF_AT(uint32_t, reinterpret_cast<void*>(firstEntry), 0x20) =
                CSF_AT(uint32_t, record, 4);
            CSF_AT(uint32_t, reinterpret_cast<void*>(firstEntry), 0x24) =
                CSF_AT(uint32_t, record, 8);
        }
        SceneLoader_AddSceneEntry(g_pCurrentScene, nullptr, firstEntry);
    }

    if (secondEntry)
    {
        void* record = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(self, nullptr, sceneIndex, secondEntry));
        if (CSF_AT(int, record, 8) >= 0)
        {
            CSF_AT(uint32_t, reinterpret_cast<void*>(secondEntry), 0x20) =
                CSF_AT(uint32_t, record, 4);
            CSF_AT(uint32_t, reinterpret_cast<void*>(secondEntry), 0x24) =
                CSF_AT(uint32_t, record, 8);
        }
        SceneLoader_AddSceneEntry(g_pCurrentScene, nullptr, secondEntry);
    }

    if (focusEntry)
    {
        void* record = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(
                self, nullptr, sceneIndex, reinterpret_cast<int>(focusEntry)));
        if (CSF_AT(int, record, 8) >= 0)
        {
            focusEntry->field_0020 = CSF_AT(uint32_t, record, 4);
            focusEntry->field_0024 = CSF_AT(uint32_t, record, 8);
        }
        SceneLoader_AddSceneEntry(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        CurrentScene_SetFocusEntry(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        SceneLoader_ConfigureFocusEntry(
            focusEntry, nullptr, focusConfiguration);

        void* relatedObject =
            reinterpret_cast<void*>(focusEntry->vGetField016c());
        void* controller =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, relatedObject, 0)[0x18 / 4])(
                    relatedObject, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x34 / 4])(
                controller, nullptr, focusMode);
    }

    SceneLoader_FinalizeSceneIndex(self, nullptr, sceneIndex);
    SceneLoader_UpdateSceneEntries(self, nullptr);
    auto* sceneRecords =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x30));
    CSF_AT(uint8_t, sceneRecords + 88 * sceneIndex, 29) = 1;
    return SceneLoader_CompleteSceneActivation(self, nullptr);
}

HOOK(0x005043F0, CSceneLoader_ActivateScene, "auto");
