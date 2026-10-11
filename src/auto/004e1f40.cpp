#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiControl_AnyRightOtherSceneTrue, 0x00450660, (CGuiControl* self, void* , int excludedObject));
CSF_FUNC(char, __fastcall, SceneSelection_CycleEligibleObject, 0x00451000, (unsigned int* self, void* , char backward));
CSF_FUNC(int, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, int requester, int flags, int priority));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e5e8_UpdateSceneAcquisition(
    C_0083e5e8* self, void* /*edx*/)
{
    const int initialState = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(initialState), 0xD0)
        && !CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x228))
    {
        void* scene = g_pCurrentScene;
        if (GetSceneMenuContext(scene, nullptr) == self->field_000c)
        {
            SceneSelection_CycleEligibleObject(
                reinterpret_cast<unsigned int*>(scene), nullptr, 0);
        }
    }

    if (!CGuiControl_AnyRightOtherSceneTrue(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene),
            nullptr, static_cast<int>(self->field_000c)))
    {
        const int state = GetGlobalState_50D190();
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(state), 0xD0))
        {
            CSF_AT(int, self, 0x18) = 5;
            self->field_0008 = 0;
            return state;
        }
    }

    C_0082f9f8_TryAcquire(
        reinterpret_cast<void*>(self->field_000c), nullptr,
        reinterpret_cast<int>(self), 0, 100);

    void* object = reinterpret_cast<void*>(self->field_000c);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, object, 0)[0x8C / 4])(
                object, nullptr, 48, 0, -1, 0);
    CSF_AT(int, self, 0x18) = 1;
    return result;
}

HOOK(0x004E1F40, C_0083e5e8_UpdateSceneAcquisition, "auto");
