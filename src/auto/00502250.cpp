#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneObjects, 0x0044F380, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_PrepareSceneCleanup, 0x0044F650, ());
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeSceneObjects, 0x00455240, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_RemoveSceneObject, 0x004DE7C0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_ClearSceneResources, 0x004FE880, ());
CSF_FUNC(int, __fastcall, SceneLoader_ClearLoadedObjects, 0x00500800, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderContext, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_ResetRenderWorld, 0x006B70D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SceneLoader_FinalizeRenderResources, 0x006BDCB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_SetRenderResetFlag, 0x006CB3A0, (void* self, void* edx, char a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_BeginRenderReset, 0x006CBBC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_EndRenderReset, 0x006CBC10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ResetRenderMember, 0x006D5BB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_SetRenderWorld, 0x006D6A40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_ClearActiveRenderObject, 0x006D81D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetRenderService, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall CSceneLoader_ProcessPendingSceneChange(
    C_0082d410* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x104);
    if (result)
    {
        void* context = reinterpret_cast<void*>(
            SceneLoader_GetRenderContext(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, context, 0)[0x94 / 4])(
                context, nullptr, CSF_AT(uint32_t, self, 0x108));

        SceneLoader_PrepareSceneCleanup();
        void* sceneState = reinterpret_cast<void*>(
            CGuiControl_GetField012C(
                static_cast<CGuiControl*>(g_pCurrentScene), nullptr));
        if (!CSF_AT(uint8_t, sceneState, 0x4))
        {
            SceneLoader_PrepareSceneObjects(g_pCurrentScene, nullptr);
            SceneLoader_FinalizeSceneObjects(g_pCurrentScene, nullptr);
        }

        CGuiControl* scene = static_cast<CGuiControl*>(g_pCurrentScene);
        uint32_t index = 0;
        if (scene->m_id.alloc)
        {
            do
            {
                void* object = reinterpret_cast<void**>(scene->field_0010)[index];
                if (object)
                {
                    void* owner = reinterpret_cast<void*>(
                        CGuiControl_GetField012C(scene, nullptr));
                    SceneLoader_RemoveSceneObject(owner, nullptr, object);
                    scene = static_cast<CGuiControl*>(g_pCurrentScene);
                }
                ++index;
            }
            while (index < static_cast<uint32_t>(scene->m_id.alloc));
        }

        SceneLoader_ClearLoadedObjects(self, nullptr);
        context = reinterpret_cast<void*>(
            SceneLoader_GetRenderContext(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        void* renderService = reinterpret_cast<void*>(
            SceneLoader_GetRenderService(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* activeObject =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, renderService, 0)[0x48 / 4])(
                    renderService, nullptr);
        SceneLoader_ClearActiveRenderObject(activeObject, nullptr);

        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x60, nullptr,
            const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

        void* first = reinterpret_cast<void*>(
            GetFirstElement(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
        void* provider = GetCachedObjectProvider(first, nullptr);
        void* referenced = GetReferencedObject(provider, nullptr);
        void* camera = GetCachedObjectProvider(referenced, nullptr);
        int world = RwCameraGetWorld(reinterpret_cast<int>(camera));
        if (world)
            RpWorldRemoveCamera(world, reinterpret_cast<int>(camera));

        SceneLoader_ClearSceneResources();

        int renderWorld =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x44 / 4])(context, nullptr);
        int renderHandle = RenderHandle_FromObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        SceneLoader_SetRenderWorld(
            reinterpret_cast<void*>(renderHandle), nullptr, renderWorld);

        SceneLoader_BeginRenderReset(context, nullptr);
        SceneLoader_ResetRenderMember(
            CSF_AT(void*, context, 0xBC), nullptr);
        SceneLoader_EndRenderReset(context, nullptr);
        SceneLoader_SetRenderResetFlag(context, nullptr, 1);

        void* worldResources =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x44 / 4])(context, nullptr);
        SceneLoader_ClearWorldResources(worldResources, nullptr);

        void* renderResources =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x40 / 4])(context, nullptr);
        SceneLoader_ClearRenderResources(renderResources, nullptr);

        worldResources =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x44 / 4])(context, nullptr);
        SceneLoader_ResetRenderWorld(worldResources, nullptr);

        renderResources =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x40 / 4])(context, nullptr);
        SceneLoader_FinalizeRenderResources(renderResources, nullptr);

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, context, 0)[0xB4 / 4])(context, nullptr);

        uint32_t pendingScene = CSF_AT(uint32_t, self, 0x18);
        if (pendingScene != static_cast<uint32_t>(-1))
        {
            uint8_t* scenes = CSF_AT(uint8_t*, self, 0x30);
            CSF_AT(uint32_t, self, 0x14) = pendingScene;
            CSF_AT(uint32_t, self, 0x20) = 1;
            scenes[88 * pendingScene + 28] = 0;
            CSF_AT(uint32_t, self, 0x18) = -1;
        }

        uint32_t state = CSF_AT(uint32_t, self, 0x20);
        result = static_cast<char>(state);
        if (state == 1)
        {
            result =
                ((char(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, self, 0)[0x34 / 4])(
                        self, nullptr, CSF_AT(uint32_t, self, 0x14));
        }
    }

    CSF_AT(uint8_t, self, 0x104) = 0;
    return result;
}

HOOK(0x00502250, CSceneLoader_ProcessPendingSceneChange, "auto");
