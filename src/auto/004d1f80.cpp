#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_RemoveFromSceneCategory, 0x0048E6B0, (C_00833908* self, void*));
CSF_FUNC(void, __fastcall, SceneActor_ResetToIdle, 0x004D1D90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CheckAndSendScaledDirection, 0x004E2B80, (void* self, void*));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Referenced by the packet (vtable reference of S_004d1d90 at the call site 0x4D200A).
[[maybe_unused]] static auto* const s_SceneActorVtbl = CSF_VTBL(0x004D1D90);

static void __fastcall SceneActor_TickFadeState(S_004d1d90* self, void* /*edx*/, float deltaTime)
{
    if (self->field_0058 == 5)
    {
        auto* sceneObj = reinterpret_cast<C_00833908*>(self->field_0014);
        float& fadeTimer = *reinterpret_cast<float*>(&self->field_00a0);
        fadeTimer = fadeTimer - deltaTime;

        void* stateObj = CSF_AT(void*, sceneObj, 0x48);
        bool finished = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, stateObj, 0)[13])(stateObj, nullptr);

        if (finished || fadeTimer < g_fZero)
        {
            SceneActor_ResetToIdle(self, nullptr);
        }
    }
    else
    {
        auto* sceneObj = reinterpret_cast<C_00833908*>(self->field_0014);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, sceneObj, 0)[35])(sceneObj, nullptr, 9, 0, -1, 0);

        uint8_t wasRemoved = self->field_0061;
        self->field_00a0 = 0x40A00000u;
        self->field_0058 = 5;
        self->field_0060 = 1;
        if (!wasRemoved)
        {
            C_00833908_RemoveFromSceneCategory(
                reinterpret_cast<C_00833908*>(self->field_0014), nullptr);
            self->field_0061 = 1;
        }

        void* sceneObjAgain = reinterpret_cast<void*>(self->field_0014);
        CheckAndSendScaledDirection(CSF_AT(void*, sceneObjAgain, 0x38), nullptr);
    }
}

HOOK(0x004D1F80, SceneActor_TickFadeState, "auto");
