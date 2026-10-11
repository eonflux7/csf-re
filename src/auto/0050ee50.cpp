#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_PrepareMenuForScene, 0x0050E7C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuManager_ApplySceneResource, 0x0050EC40, (void* self, void* edx, int a4, int a5));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_SelectArchive, 0x00698960, (char* String1));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_EmptyNameByte, 0x0082E65C);
CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_MenuHeightScale, 0x0083F250);

static int __fastcall CMenuManager_PrepareSceneMenus(
    CMenuManager* self, void* /*edx*/, void* scene)
{
    void* gameState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (CSF_AT(uint8_t, gameState, 0xA5C))
        SceneLoader_SelectArchive(reinterpret_cast<char*>(GetGlobalIntPtr()));

    SceneLoader_PrepareMenuForScene(self, nullptr);

    void* controller = reinterpret_cast<void*>(self->field_0014);
    void* control = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0x54 / 4])(controller, nullptr);
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, control, 0)[0x0C / 4])(control, nullptr, 0);

    controller = reinterpret_cast<void*>(self->field_0014);
    control = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, controller, 0)[0x54 / 4])(controller, nullptr);
    ((void (__fastcall*)(void*, void*, int))
        CSF_AT(void**, control, 0)[0x0C / 4])(control, nullptr, 1);

    gameState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int resource = SceneLoader_GetResourceById(
        reinterpret_cast<uint8_t*>(gameState) + 0x22A4, nullptr, 0);
    CMenuManager_ApplySceneResource(self, nullptr, resource, 0);

    gameState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    resource = SceneLoader_GetResourceById(
        reinterpret_cast<uint8_t*>(gameState) + 0x22A4, nullptr, 1);
    CMenuManager_ApplySceneResource(self, nullptr, resource, 1);

    self->vSlot80();

    if (self->m_pGuiRoot)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
        void* root = reinterpret_cast<void*>(self->m_pGuiRoot);
        ((void (__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, root, 0)[0x20 / 4])(
                root, nullptr, CSF_AT(uint32_t, settings, 0x200));

        void* display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        float width = static_cast<float>(
            ((int (__fastcall*)(void*, void*))
                CSF_AT(void**, display, 0)[0x20 / 4])(display, nullptr));

        display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        int height = ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x24 / 4])(display, nullptr);

        root = reinterpret_cast<void*>(self->m_pGuiRoot);
        float heightScale = static_cast<float>(height) * g_MenuHeightScale;
        float widthScale = width * g_MenuWidthScale;
        ((void (__fastcall*)(void*, void*, float, float))
            CSF_AT(void**, root, 0)[0x28 / 4])(
                root, nullptr, widthScale, heightScale);
    }

    if (scene && self->m_pFader)
    {
        void* sceneObject = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, scene, 0)[0x20 / 4])(scene, nullptr);
        if (sceneObject)
        {
            const char* name = ((const char* (__fastcall*)(void*, void*))
                CSF_AT(void**, sceneObject, 0)[0x190 / 4])(
                    sceneObject, nullptr);
            if (static_cast<uint8_t>(name[0]) != g_EmptyNameByte)
            {
                void** faderVtable = CSF_AT(
                    void**, reinterpret_cast<void*>(self->m_pFader), 0);
                name = ((const char* (__fastcall*)(void*, void*))
                    CSF_AT(void**, sceneObject, 0)[0x190 / 4])(
                        sceneObject, nullptr);
                ((void (__fastcall*)(void*, void*, const char*))
                    faderVtable[0x04 / 4])(
                        reinterpret_cast<void*>(self->m_pFader), nullptr, name);
            }
        }
    }

    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x8C / 4])(self, nullptr);

    int result = ((int (__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
    if (!result)
    {
        controller = reinterpret_cast<void*>(self->field_0014);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, controller, 0)[0x9C / 4])(controller, nullptr, 1);

        controller = reinterpret_cast<void*>(self->field_0014);
        return ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0xA8 / 4])(controller, nullptr);
    }
    return result;
}

HOOK(0x0050EE50, CMenuManager_PrepareSceneMenus, "auto");
