#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::int32_t, g_NextUniqueId, 0x008A669C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, SceneLoader_PrepareGlobalState, 0x00442A00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_AddSceneEntry, 0x00453150, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareCurrentScene, 0x00455240, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, SceneLoader_FindEntryRecord, 0x00500410, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeLoadedEntries, 0x00500800, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_LoadNamedEntries, 0x00500AE0, (void* self, void* edx, int a2, char a3, int a4, uint32_t* a5, uint32_t* a6, uint32_t* a7, int a8, int a9));
CSF_FUNC(int, __fastcall, SceneLoader_UpdateLoadState, 0x00503EF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static void SceneLoader_InitializeNotification(void* record, int kind)
{
    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;
    CSF_AT(int, record, 0x04) = kind;
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(uint8_t, record, 0x30) = 0;
}

static int __fastcall CSceneLoader_LoadSceneEntries(
    CSceneLoader* self, void* /*edx*/, int sceneIndex,
    C_00834050* focusEntry, int secondaryEntry, int tertiaryEntry,
    int* focusSettings, int focusMode)
{
    uint32_t focusId = -1;
    uint32_t secondaryId = -1;
    uint32_t tertiaryId = -1;

    SceneLoader_PrepareGlobalState(
        reinterpret_cast<void*>(GetValue_8afc80()), nullptr);
    SceneLoader_PrepareCurrentScene(g_pCurrentScene, nullptr);

    const int recordOffset = 88 * sceneIndex;
    auto* sceneRecord =
        reinterpret_cast<uint8_t*>(self->m_scenesBegin) + recordOffset;
    void* sceneName;
    if (CSF_AT(uint32_t, sceneRecord, 0x18) < 0x10u)
        sceneName = sceneRecord + 4;
    else
        sceneName = CSF_AT(void*, sceneRecord, 0x04);

    SceneLoader_LoadNamedEntries(
        self, nullptr, reinterpret_cast<int>(sceneName), 1,
        reinterpret_cast<int>(focusEntry), &focusId, &secondaryId,
        &tertiaryId, secondaryEntry, tertiaryEntry);

    if (focusId == uint32_t(-1))
    {
        const int32_t uniqueId = g_NextUniqueId;
        g_NextUniqueId = static_cast<int32_t>(
            static_cast<uint32_t>(uniqueId) + 1);
        focusEntry->field_0020 = uniqueId;
        focusEntry->field_0024 = -1;
    }
    else if (focusEntry)
    {
        void* entryRecord = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(
                self, nullptr, sceneIndex, reinterpret_cast<int>(focusEntry)));
        CSF_AT(uint32_t, entryRecord, 0x04) = focusId;
        focusEntry->field_0020 = focusId;
        focusEntry->field_0024 = CSF_AT(uint32_t, entryRecord, 0x08);
    }

    if (secondaryId != uint32_t(-1) && secondaryEntry)
    {
        void* entryRecord = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(
                self, nullptr, sceneIndex, secondaryEntry));
        CSF_AT(uint32_t, entryRecord, 0x04) = secondaryId;
        CSF_AT(uint32_t, reinterpret_cast<void*>(secondaryEntry), 0x20) = secondaryId;
        CSF_AT(uint32_t, reinterpret_cast<void*>(secondaryEntry), 0x24) =
            CSF_AT(uint32_t, entryRecord, 0x08);
    }

    if (tertiaryId != uint32_t(-1) && tertiaryEntry)
    {
        void* entryRecord = reinterpret_cast<void*>(
            SceneLoader_FindEntryRecord(
                self, nullptr, sceneIndex, tertiaryEntry));
        CSF_AT(uint32_t, entryRecord, 0x04) = tertiaryId;
        CSF_AT(uint32_t, reinterpret_cast<void*>(tertiaryEntry), 0x20) = tertiaryId;
        CSF_AT(uint32_t, reinterpret_cast<void*>(tertiaryEntry), 0x24) =
            CSF_AT(uint32_t, entryRecord, 0x08);
    }

    if (secondaryEntry)
        SceneLoader_AddSceneEntry(g_pCurrentScene, nullptr, secondaryEntry);
    if (tertiaryEntry)
        SceneLoader_AddSceneEntry(g_pCurrentScene, nullptr, tertiaryEntry);

    if (focusEntry)
    {
        SceneLoader_AddSceneEntry(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        CurrentScene_SetFocusEntry(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        SceneLoader_ConfigureFocusEntry(focusEntry, nullptr, focusSettings);

        void* relatedObject =
            reinterpret_cast<void*>(focusEntry->vGetField016c());
        void* controller = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, relatedObject, 0)[0x18 / 4])(
                    relatedObject, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x34 / 4])(
                controller, nullptr, focusMode);
    }

    SceneLoader_FinalizeLoadedEntries(self, nullptr);
    CSF_AT(uint8_t,
        reinterpret_cast<uint8_t*>(self->m_scenesBegin) + recordOffset, 0x1D) = 1;
    SceneLoader_UpdateLoadState(self, nullptr);

    alignas(4) uint8_t notification[0x38] = {};
    CSF_AT(int, notification, 0x34) = recordOffset;

    if (CSF_AT(uint8_t,
            reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 207))
    {
        SceneLoader_InitializeNotification(notification, 43);
        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, notification, 1);
    }

    SceneLoader_InitializeNotification(notification, 0);
    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, notification, 1));
}

HOOK(0x005041D0, CSceneLoader_LoadSceneEntries, "auto");
