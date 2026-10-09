#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_Update, 0x004814A0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_FindStateIndex, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CCharActionFsm_Update(
    CCharActionFsm* self, void* /*edx*/, float deltaTime)
{
    void* activeState = nullptr;
    const uint32_t activeIndex = self->field_0090;
    if (activeIndex != static_cast<uint32_t>(-1))
    {
        activeState = reinterpret_cast<void**>(self->field_0078)[activeIndex];
        if (activeState)
        {
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, activeState, 0)[0x28 / 4])(
                    activeState, nullptr, deltaTime);
        }
    }

    void* secondaryState = reinterpret_cast<void*>(self->field_009c);
    if (secondaryState && secondaryState != activeState)
    {
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, secondaryState, 0)[0x28 / 4])(
                secondaryState, nullptr, deltaTime);
    }

    const int stateIndex = CCharWeaponFsm_FindStateIndex(self, nullptr, 28);
    if (stateIndex != -1)
    {
        void* state = reinterpret_cast<void**>(self->field_0078)[stateIndex];
        if (CSF_AT(uint32_t, state, 0x80))
        {
            if (state && state != activeState)
            {
                ((void(__fastcall*)(void*, void*, float))
                    CSF_AT(void**, state, 0)[0x28 / 4])(
                        state, nullptr, deltaTime);
            }
        }
    }

    if (self->field_00d0 > g_fZero)
    {
        const uint32_t sceneContext =
            GetSceneMenuContext(g_pCurrentScene, nullptr);
        if (sceneContext == self->field_0008)
        {
            void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* timedObject = CSF_AT(void*, globalState, 0x74);
            if (timedObject)
            {
                if (((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, timedObject, 0)[0x10 / 4])(
                            timedObject, nullptr) == -1
                    || ((int(__fastcall*)(void*, void*))
                        CSF_AT(void**, timedObject, 0)[0x10 / 4])(
                            timedObject, nullptr) == -2)
                {
                    self->field_00d0 = -1.0f;
                }
                else
                {
                    self->field_00d0 = self->field_00d0 - deltaTime;
                    if (self->field_00d0 <= g_fZero)
                    {
                        ((void(__fastcall*)(void*, void*))
                            CSF_AT(void**, timedObject, 0)[0x8 / 4])(
                                timedObject, nullptr);
                    }
                }
            }
        }
    }

    return CCharWeaponFsm_Update(self, nullptr, deltaTime);
}

HOOK(0x00488320, CCharActionFsm_Update, "auto");
