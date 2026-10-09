#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, HudUpdateSceneMenuVisibility, 0x0042FA50, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareCurrentScene, 0x0044F380, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearSceneContents, 0x0044FB50, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SelectScene, 0x00450630, (int a1, int a2));
CSF_FUNC(void, __fastcall, SceneLoader_DetachPersistentActor, 0x004530D0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectByHandle, 0x00453560, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_SetSceneActive, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(void*, __fastcall, SceneLoader_SetActorTransitionState, 0x00484420, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneResources, 0x004FFE00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_LoadSceneResources, 0x00503000, (void* self, void* edx, char* a2, int a4, int a5));
CSF_FUNC(int, __fastcall, SceneLoader_RestoreActorsDefault, 0x005041D0, (void* self, void* edx, int sceneIndex, void* context, int firstActor, int secondActor, int* position, int heading));
CSF_FUNC(int, __fastcall, SceneLoader_RestoreActorsAlternate, 0x005043F0, (void* self, void* edx, int a1, void* a3, int a4, int a5, int* a6, int a7));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ResetTransitionManager, 0x0051A550, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneService, 0x00652010, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_ApplySceneOptions, 0x006980D0, (char* String1));
CSF_FUNC(void, __cdecl, SceneLoader_ResetSceneOptions, 0x006981B0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetOptionsByte, 0x00698600, (char a1));
CSF_FUNC(void, __cdecl, SceneLoader_LoadSceneOptions, 0x00698960, (char* String1));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, SceneLoader_SetSceneText, 0x0069BD90, (void* self, void* edx, unsigned int a2, char* a3));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CSceneLoaderBase_ChangeScene(
    C_0082d410* self, void* /*edx*/, int sceneIndex, char discardActors)
{
    alignas(4) std::uint8_t removalRecord[0x38]{};
    CSF_AT(void**, removalRecord, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(std::uint32_t, removalRecord, 0x04) = 19;
    CSF_AT(void**, removalRecord, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, removalRecord, 0x0C) = -1;
    CSF_AT(int, removalRecord, 0x10) = -1;
    CSF_AT(void**, removalRecord, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, removalRecord, 0x18) = -1;
    CSF_AT(int, removalRecord, 0x1C) = -1;

    // Reserve the full region available to the original position argument.
    int savedPosition[0x24 / sizeof(int)]{};
    float savedHeading = 0.0f;
    void* savedContext = nullptr;
    void* firstActor = nullptr;
    void* secondActor = nullptr;

    for (int index = CSF_AT(int, g_pCurrentScene, 0x18) - 1;
         index >= 0; --index)
    {
        void* object = CSF_AT(void**, g_pCurrentScene, 0x10)[index];
        if (object &&
            static_cast<std::uint8_t>(
                Scene_IsObjectRemovable(CSF_AT(int, object, 0x50))) != 0)
        {
            CSF_AT(int, removalRecord, 0x0C) = CSF_AT(int, object, 0x20);
            auto* handle = static_cast<std::uint8_t*>(object) + 0x1C;
            CSF_AT(int, removalRecord, 0x10) = CSF_AT(int, handle, 0x08);

            void* dispatcher = reinterpret_cast<void*>(GetGlobalValue_51F740());
            ((void (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, dispatcher, 0)[0])(
                    dispatcher, nullptr, removalRecord);
            SceneLoader_RemoveObjectByHandle(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(handle));
        }
    }

    void* gameState = reinterpret_cast<void*>(GetValue8B0554());
    SceneLoader_ResetTransitionManager(
        CSF_AT(void*, gameState, 8132), nullptr);

    char savedOptions = GetOptionsByteAt2();
    SceneLoader_SetOptionsByte(0);

    auto* actorList = static_cast<std::uint8_t*>(g_pCurrentScene) + 0x60;
    if (!discardActors)
    {
        savedContext = reinterpret_cast<void*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));
        if (savedContext)
        {
            void* actionFsm = CSF_AT(void*, savedContext, 0x44);
            if (actionFsm)
                SceneLoader_SetActorTransitionState(actionFsm, nullptr, 1);

            int* position = reinterpret_cast<int*>(
                GetObjectPositionVector(savedContext, nullptr));
            savedPosition[0] = position[0];
            savedPosition[1] = position[1];
            savedPosition[2] = position[2];

            void* orientation = reinterpret_cast<void*>(
                reinterpret_cast<C_008342e8*>(savedContext)->vGetField016c());
            void* headingSource =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, orientation, 0)[0x18 / 4])(
                        orientation, nullptr);
            savedHeading = static_cast<float>(
                ((double (__fastcall*)(void*, void*))
                    CSF_AT(void**, headingSource, 0)[0x28 / 4])(
                        headingSource, nullptr));

            SceneLoader_DetachPersistentActor(g_pCurrentScene, nullptr, nullptr);
        }

        std::uint32_t index = 0;
        if (CSF_AT(std::uint32_t, actorList, 0x08) != 0)
        {
            do
            {
                void* actor = CSF_AT(void**, actorList, 0)[index];
                if (actor && actor != savedContext &&
                    C_GetByte023C(static_cast<char*>(actor), nullptr))
                {
                    if (firstActor)
                        secondActor = actor;
                    else
                        firstActor = actor;
                    SceneLoader_DetachPersistentActor(
                        g_pCurrentScene, nullptr, actor);
                }
                else
                {
                    ++index;
                }
            } while (index < CSF_AT(std::uint32_t, actorList, 0x08));
        }
    }

    void* oldScene = g_pCurrentScene;
    SceneLoader_SelectScene(sceneIndex, 0);
    SceneLoader_SetSceneActive(oldScene, nullptr, 0);
    SceneLoader_ClearSceneContents(oldScene, nullptr);
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, self, 0)[0x3C / 4])(self, nullptr, 1);
    SceneLoader_PrepareCurrentScene(g_pCurrentScene, nullptr);

    const std::uint32_t entryOffset =
        88 * static_cast<std::uint32_t>(sceneIndex);
    auto* entry = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x30)) + entryOffset;
    char* sceneName = CSF_AT(std::uint32_t, entry, 0x18) < 0x10u
        ? reinterpret_cast<char*>(entry + 4)
        : CSF_AT(char*, entry, 4);
    SceneLoader_LoadSceneResources(
        self, nullptr, sceneName, 1, CSF_AT(int, entry, 0x20));
    SceneLoader_FinalizeSceneResources(self, nullptr);

    entry = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x30)) + entryOffset;
    int headingBits;
    std::memcpy(&headingBits, &savedHeading, sizeof(headingBits));
    if (CSF_AT(std::uint8_t, entry, 0x1D))
    {
        SceneLoader_RestoreActorsAlternate(
            self, nullptr, sceneIndex, discardActors ? nullptr : savedContext,
            reinterpret_cast<int>(firstActor), reinterpret_cast<int>(secondActor),
            savedPosition, headingBits);
    }
    else
    {
        SceneLoader_RestoreActorsDefault(
            self, nullptr, sceneIndex, discardActors ? nullptr : savedContext,
            reinterpret_cast<int>(firstActor), reinterpret_cast<int>(secondActor),
            savedPosition, headingBits);
    }

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(std::uint8_t, settings, 2652))
    {
        SceneLoader_ResetSceneOptions();
        char* options = reinterpret_cast<char*>(GetGlobalIntPtr());
        SceneLoader_LoadSceneOptions(options);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));
    void* sceneController =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, sceneController, 0)[0x24 / 4])(
            sceneController, nullptr);

    gameState = reinterpret_cast<void*>(GetValue8B0554());
    if (CSF_AT(int, gameState, 5716))
    {
        auto* stateData = static_cast<std::uint8_t*>(gameState) + 84;
        char* text = CSF_AT(std::uint32_t, stateData, 5636) < 0x10u
            ? reinterpret_cast<char*>(stateData + 5616)
            : CSF_AT(char*, stateData, 5616);
        void* textDatabase = reinterpret_cast<void*>(GetGlobalValue_69B700());
        SceneLoader_SetSceneText(textDatabase, nullptr, 4u, text);
    }

    char* options = reinterpret_cast<char*>(GetGlobalIntPtr());
    SceneLoader_ApplySceneOptions(options);
    SceneLoader_SetOptionsByte(savedOptions);

    auto* loaderBytes = reinterpret_cast<std::uint8_t*>(self);
    void* entries = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x30));
    CSF_AT(std::uint32_t, self, 0x0C) =
        static_cast<std::uint32_t>(sceneIndex);
    AssignStringSubstring(
        loaderBytes + 0x7C, nullptr,
        static_cast<std::uint8_t*>(entries) + entryOffset, 0, -1);

    entry = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x30)) + entryOffset;
    ++CSF_AT(std::uint32_t, entry, 32);
    AssignStringSubstring(
        loaderBytes + 0x60, nullptr, loaderBytes + 0x7C, 0, -1);

    globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneService = reinterpret_cast<void*>(
        SceneLoader_GetSceneService(globalObject, nullptr));
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, sceneService, 0)[180 / 4])(sceneService, nullptr);

    globalObject = reinterpret_cast<void*>(GetGlobalInt());
    accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, accessor, 0)[32 / 4])(accessor, nullptr, 1);

    alignas(4) std::uint8_t completionRecord[0x34]{};
    CSF_AT(void**, completionRecord, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, completionRecord, 0x14) = CSF_VTBL(0x0082D448);
    std::memset(completionRecord + 0x20, 0, 16);
    CSF_AT(std::uint8_t, completionRecord, 0x30) = 0;
    CSF_AT(void**, completionRecord, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(int, completionRecord, 0x0C) = -1;
    CSF_AT(int, completionRecord, 0x10) = -1;
    CSF_AT(int, completionRecord, 0x18) = -1;
    CSF_AT(int, completionRecord, 0x1C) = -1;
    CSF_AT(std::uint32_t, completionRecord, 0x04) = 36;

    void* notificationTarget =
        reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(
        notificationTarget, nullptr, completionRecord, 1);

    CSF_AT(int, self, 0x18) = -1;
    void* hud = reinterpret_cast<void*>(GetGlobal8afc78());
    return HudUpdateSceneMenuVisibility(hud, nullptr);
}

HOOK(0x00504DF0, CSceneLoaderBase_ChangeScene, "auto");
