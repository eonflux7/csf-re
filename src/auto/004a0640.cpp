#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004503E0_ForwardWithScratch, 0x00452210, (S_004503e0* self, void* , C_008342e8* a2, float a3));
CSF_FUNC(uint32_t*, __stdcall, HudAlertIcons_UpdateIcon, 0x0049FD30, (int a2));
CSF_FUNC(void, __fastcall, HudAlertIcons_RemoveIcon, 0x004A0490, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, HudAlertIcons_UpdateAlternateDisplay, 0x004A2850, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));

static int __fastcall S_004a0490_UpdateAlertIcons(S_004a0490* self, void* /*edx*/)
{
    const uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!context)
        return 0;

    for (int32_t index = static_cast<int32_t>(self->field_0018 - 1);
         index >= 0; --index)
    {
        auto* icon = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint32_t*>(self->field_0010)[index]);

        if (CSF_AT(uint32_t, icon, 0x14))
        {
            if (CSF_AT(uint8_t, icon, 0x18))
            {
                // The packet annotates this direct callee as a vtable reference.
                (void)CSF_VTBL(0x004A0490);
                HudAlertIcons_RemoveIcon(self, nullptr, icon);
            }
            else
            {
                CSF_AT(uint8_t, icon, 0x18) = 1;
            }
        }
    }

    for (uint32_t index = 0; index < self->field_0018; ++index)
    {
        void* icon = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0010)[index]);
        const int particleHandle = CSF_AT(int, icon, 0x14);
        if (particleHandle)
        {
            void* controller = reinterpret_cast<void*>(
                FxParticleManager_GetParticleController(particleHandle));
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0x24 / 4])(
                    controller, nullptr, 0);
        }
    }

    void* collection = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* firstElement = reinterpret_cast<void*>(GetFirstElement(collection, nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    GetReferencedObject(provider, nullptr);

    CSF_AT(uint32_t, self, 0x28) = 0;
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0))
    {
        return HudAlertIcons_UpdateAlternateDisplay(
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x30)), nullptr);
    }

    void* scene = g_pCurrentScene;
    uint32_t sceneIndex = 0;
    while (sceneIndex < CSF_AT(uint32_t, scene, 0x28))
    {
        void* sceneObject = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, scene, 0x20))[sceneIndex]);

        if (sceneObject && (CSF_AT(uint32_t, sceneObject, 0x170) & 0x2000))
        {
            void* matchingIcon = nullptr;
            for (uint32_t iconIndex = 0; iconIndex < self->field_0018; ++iconIndex)
            {
                void* icon = reinterpret_cast<void*>(
                    reinterpret_cast<uint32_t*>(self->field_0010)[iconIndex]);
                if (CSF_AT(uint32_t, icon, 0x04) ==
                    CSF_AT(uint32_t, sceneObject, 0x20))
                {
                    matchingIcon = icon;
                    break;
                }
            }

            if (matchingIcon)
            {
                auto* resolvedObject = reinterpret_cast<C_008342e8*>(
                    CHandleTable_Resolve(
                        g_pCurrentScene, nullptr, reinterpret_cast<int>(matchingIcon)));
                if (resolvedObject &&
                    S_004503E0_ForwardWithScratch(
                        reinterpret_cast<S_004503e0*>(g_pCurrentScene),
                        nullptr, resolvedObject, 6000.0f))
                {
                    HudAlertIcons_UpdateIcon(reinterpret_cast<int>(matchingIcon));

                    void* controller = reinterpret_cast<void*>(
                        FxParticleManager_GetParticleController(
                            CSF_AT(int, matchingIcon, 0x14)));
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, controller, 0)[0x24 / 4])(
                            controller, nullptr, 1);

                    if (CSF_AT(uint32_t, self, 0x28) >=
                        CSF_AT(uint32_t, self, 0x24))
                    {
                        // The packet annotates this direct callee as a vtable reference.
                        (void)CSF_VTBL(0x0041E5F0);
                        S_0041e5f0_GrowArrayCapacity(
                            reinterpret_cast<S_0041e5f0*>(
                                reinterpret_cast<uint8_t*>(self) + 0x20),
                            nullptr, CSF_AT(int, self, 0x2C));
                    }

                    const uint32_t count = CSF_AT(uint32_t, self, 0x28);
                    auto* destination = reinterpret_cast<uint32_t*>(
                        CSF_AT(uint32_t, self, 0x20)) + count;
                    CSF_AT(uint32_t, self, 0x28) = count + 1;
                    *destination = reinterpret_cast<uint32_t>(matchingIcon);
                }
            }
        }

        scene = g_pCurrentScene;
        ++sceneIndex;
    }

    return static_cast<int>(sceneIndex);
}

HOOK(0x004A0640, S_004a0490_UpdateAlertIcons, "auto");
