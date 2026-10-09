#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, HudUpdateSceneMenuVisibility, 0x0042FA50, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneRebuild, 0x0044F380, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_ResetSceneGlobals, 0x0044F650, ());
CSF_FUNC(void, __fastcall, C_0044f680_SetFlag128, 0x0044F680, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneRebuild, 0x0044F690, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_RebuildSceneObjects, 0x0044FB50, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneLoader_DetachSceneEntry, 0x004530D0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, SceneLoader_AttachSceneEntry, 0x00453150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectReference, 0x00453560, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_SetSceneRebuildFlag, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, SceneLoader_RefreshSceneState, 0x00455240, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(void*, __fastcall, SceneLoader_SetActionFsmMode, 0x00484420, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_SetObjectPosition, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeGlobalSceneState, 0x004FBE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, SceneLoader_PrepareGlobalSceneState, 0x004FC440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_RefreshGlobalSceneResources, 0x004FE880, ());
CSF_FUNC(void*, __fastcall, SceneLoader_LoadScene, 0x004FF930, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_SaveSceneEntry, 0x005003A0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderContext, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(void, __cdecl, SceneLoader_RefreshEnabledOptions, 0x006981B0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetOptionsByte, 0x00698600, (char a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_RefreshRenderScene, 0x006B70D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareRenderCamera, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_FinalizeRenderCamera, 0x006BDCB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetRenderContextMode, 0x006CB3A0, (void* self, void* edx, char a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_PrepareRenderContext, 0x006CBBC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeRenderContext, 0x006CBC10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_RefreshRenderResource, 0x006D5BB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_AssignRenderScene, 0x006D6A40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareRenderResource, 0x006D81D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderResourceOwner, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_RefreshRenderWorld, 0x007512A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CSceneLoaderBase_ReloadScene(
    C_0082d410* self, void* /*edx*/, int sceneIndex)
{
    if (CSF_AT(int, self, 0x0C) == sceneIndex)
    {
        uint32_t reloadRecord[0x34 / sizeof(uint32_t)]{};
        reloadRecord[2] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        reloadRecord[5] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        reloadRecord[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
        reloadRecord[3] = -1;
        reloadRecord[4] = -1;
        reloadRecord[6] = -1;
        reloadRecord[7] = -1;
        reloadRecord[1] = 37;
        CSF_AT(char, reloadRecord, 0x30) = 0;

        void* recordDispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(recordDispatcher, nullptr, reloadRecord, 1);

        void* renderContext = reinterpret_cast<void*>(
            SceneLoader_GetRenderContext(reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        SceneLoader_PrepareGlobalSceneState(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

        void* resourceOwner = reinterpret_cast<void*>(
            SceneLoader_GetRenderResourceOwner(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* renderResource =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, resourceOwner, 0)[0x48 / 4])(
                resourceOwner, nullptr);
        SceneLoader_PrepareRenderResource(renderResource, nullptr);

        C_0044f680_SetFlag128(g_pCurrentScene, nullptr);

        uint32_t removalRecord[0x38 / sizeof(uint32_t)]{};
        removalRecord[2] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        removalRecord[5] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        removalRecord[3] = -1;
        removalRecord[4] = -1;
        removalRecord[6] = -1;
        removalRecord[7] = -1;
        removalRecord[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
        removalRecord[1] = 19;
        CSF_AT(char, removalRecord, 0x30) = 0;

        for (int index = CSF_AT(int, g_pCurrentScene, 0x18) - 1; index >= 0; --index)
        {
            void* object = CSF_AT(void**, g_pCurrentScene, 0x10)[index];
            if (object &&
                static_cast<uint8_t>(Scene_IsObjectRemovable(CSF_AT(int, object, 0x50))))
            {
                removalRecord[3] = CSF_AT(uint32_t, object, 0x20);
                void* reference = static_cast<uint8_t*>(object) + 0x1C;
                removalRecord[4] = CSF_AT(uint32_t, reference, 0x08);

                void* dispatcher = reinterpret_cast<void*>(GetGlobalValue_51F740());
                ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, dispatcher, 0)[0])(
                    dispatcher, nullptr, removalRecord);
                SceneLoader_RemoveObjectReference(
                    g_pCurrentScene, nullptr, reinterpret_cast<int>(reference));
            }
        }

        int savedPosition[0x20 / sizeof(int)]{};
        float savedAngle = 0.0f;
        C_008342e8* focusEntry = reinterpret_cast<C_008342e8*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));

        if (focusEntry)
        {
            SceneLoader_SaveSceneEntry(
                self, nullptr, CSF_AT(int, self, 0x0C),
                reinterpret_cast<uint32_t*>(focusEntry));

            if (focusEntry->field_0044)
                SceneLoader_SetActionFsmMode(
                    reinterpret_cast<void*>(focusEntry->field_0044), nullptr, 1);

            const int* position = reinterpret_cast<const int*>(
                GetObjectPositionVector(focusEntry, nullptr));
            savedPosition[0] = position[0];
            savedPosition[1] = position[1];
            savedPosition[2] = position[2];

            void* relatedObject = reinterpret_cast<void*>(focusEntry->vGetField016c());
            void* orientation =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, relatedObject, 0)[0x18 / 4])(
                    relatedObject, nullptr);
            savedAngle = static_cast<float>(
                ((double(__fastcall*)(void*, void*))CSF_AT(void**, orientation, 0)[0x28 / 4])(
                    orientation, nullptr));

            SceneLoader_DetachSceneEntry(g_pCurrentScene, nullptr, nullptr);
        }

        void* entries = static_cast<uint8_t*>(g_pCurrentScene) + 0x60;
        C_008342e8* firstPreservedEntry = nullptr;
        C_008342e8* lastPreservedEntry = nullptr;
        uint32_t index = 0;

        if (CSF_AT(uint32_t, entries, 0x08) > 0)
        {
            do
            {
                C_008342e8* entry = CSF_AT(C_008342e8**, entries, 0)[index];
                if (entry && entry != focusEntry &&
                    C_GetByte023C(reinterpret_cast<char*>(entry), nullptr))
                {
                    if (firstPreservedEntry)
                        lastPreservedEntry = entry;
                    else
                        firstPreservedEntry = entry;

                    SceneLoader_SaveSceneEntry(
                        self, nullptr, CSF_AT(int, self, 0x0C),
                        reinterpret_cast<uint32_t*>(entry));
                    SceneLoader_DetachSceneEntry(g_pCurrentScene, nullptr, entry);
                    // Detaching removes this element; process its replacement at the same index.
                }
                else
                {
                    ++index;
                }
            }
            while (index < CSF_AT(uint32_t, entries, 0x08));
        }

        CSF_AT(int, self, 0x1C) = g_CurrentSceneIndex;
        SceneLoader_LoadScene(self, nullptr, sceneIndex);
        SceneLoader_PrepareSceneRebuild(g_pCurrentScene, nullptr);
        SceneLoader_SetSceneRebuildFlag(g_pCurrentScene, nullptr, 0);
        SceneLoader_RebuildSceneObjects(g_pCurrentScene, nullptr);
        SceneLoader_ResetSceneGlobals();
        SceneLoader_FinalizeSceneRebuild(g_pCurrentScene, nullptr);
        SceneLoader_RefreshSceneState(g_pCurrentScene, nullptr);

        if (firstPreservedEntry)
            SceneLoader_AttachSceneEntry(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(firstPreservedEntry));
        if (lastPreservedEntry)
            SceneLoader_AttachSceneEntry(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(lastPreservedEntry));

        SceneLoader_AttachSceneEntry(g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        CurrentScene_SetFocusEntry(g_pCurrentScene, nullptr, reinterpret_cast<int>(focusEntry));
        SceneLoader_SetObjectPosition(focusEntry, nullptr, savedPosition);

        void* relatedObject = reinterpret_cast<void*>(focusEntry->vGetField016c());
        void* orientation =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, relatedObject, 0)[0x18 / 4])(
                relatedObject, nullptr);
        ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, orientation, 0)[0x34 / 4])(
            orientation, nullptr, savedAngle);

        void* sceneState = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* firstElement = reinterpret_cast<void*>(GetFirstElement(sceneState, nullptr));
        void* provider = GetCachedObjectProvider(firstElement, nullptr);
        if (GetSceneMenuContext(g_pCurrentScene, nullptr))
            ObjectProvider_SetUpdateMode(provider, nullptr, 0);
        else
            ObjectProvider_SetUpdateMode(provider, nullptr, 4);

        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x60, nullptr,
            reinterpret_cast<char*>(&g_byte_82E65C), 0);

        sceneState = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        firstElement = reinterpret_cast<void*>(GetFirstElement(sceneState, nullptr));
        provider = GetCachedObjectProvider(firstElement, nullptr);
        void* referencedObject = GetReferencedObject(provider, nullptr);
        void* camera = GetCachedObjectProvider(referencedObject, nullptr);
        int world = RwCameraGetWorld(reinterpret_cast<int>(camera));
        if (world)
            RpWorldRemoveCamera(world, reinterpret_cast<int>(camera));

        SceneLoader_RefreshGlobalSceneResources();

        int renderScene =
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0x44 / 4])(
                renderContext, nullptr);
        void* renderHandle = reinterpret_cast<void*>(
            RenderHandle_FromObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        SceneLoader_AssignRenderScene(renderHandle, nullptr, renderScene);
        SceneLoader_PrepareRenderContext(renderContext, nullptr);
        SceneLoader_RefreshRenderResource(CSF_AT(void*, renderContext, 0xBC), nullptr);
        SceneLoader_FinalizeRenderContext(renderContext, nullptr);
        SceneLoader_SetRenderContextMode(renderContext, nullptr, 1);

        void* renderWorld =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0x44 / 4])(
                renderContext, nullptr);
        SceneLoader_RefreshRenderWorld(renderWorld, nullptr);

        void* renderCamera =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0x40 / 4])(
                renderContext, nullptr);
        SceneLoader_PrepareRenderCamera(renderCamera, nullptr);

        renderScene =
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0x44 / 4])(
                renderContext, nullptr);
        SceneLoader_RefreshRenderScene(reinterpret_cast<void*>(renderScene), nullptr);

        renderCamera =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0x40 / 4])(
                renderContext, nullptr);
        SceneLoader_FinalizeRenderCamera(renderCamera, nullptr);

        ((void(__fastcall*)(void*, void*))CSF_AT(void**, renderContext, 0)[0xB4 / 4])(
            renderContext, nullptr);

        SceneLoader_FinalizeGlobalSceneState(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

        CSF_AT(int, self, 0x10) = CSF_AT(int, self, 0x0C);
        CSF_AT(int, self, 0x0C) = -1;
    }

    char optionsByte = GetOptionsByteAt2();
    SceneLoader_SetOptionsByte(0);
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 2652))
        SceneLoader_RefreshEnabledOptions();
    SceneLoader_SetOptionsByte(optionsByte);

    void* sceneEntry = reinterpret_cast<uint8_t*>(CSF_AT(void*, self, 0x30))
        + 88 * sceneIndex;
    CSF_AT(uint8_t, sceneEntry, 28) = 0;

    return HudUpdateSceneMenuVisibility(reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);
}

HOOK(0x005029A0, CSceneLoaderBase_ReloadScene, "auto");
