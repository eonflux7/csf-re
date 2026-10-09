#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ColorAnimBufferInit, 0x00412AB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCharacterToggleState, 0x00496DC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharActionFsm_SetCharacterToggleState, 0x004975E0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D9F0, 0x0050D9F0, (void* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, C_00833908_SetSceneResourceIfSceneMatches, 0x00525A50, (C_00833908* self, void* , int sceneId, int resourceId));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static void __fastcall C_00833ee8_UpdateActionResource(
    C_00833ee8* self, void* /*edx*/, float elapsedTime)
{
    if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(self, nullptr)))
        return;
    if (static_cast<uint8_t>(SceneObj_IsInactive(self, nullptr)))
        return;

    const float accumulatedTime = elapsedTime + self->field_02f8;
    self->field_02f8 = accumulatedTime;
    if (accumulatedTime < g_flt_0082DC84)
        return;
    self->field_02f8 = accumulatedTime - g_flt_0082DC84;

    float* parameters =
        reinterpret_cast<float*>(GetGlobalValue_8BC1E0() + 8);
    float change;
    if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
        change = parameters[1564];
    else
        change = parameters[1553];
    change *= g_flt_0082DC84;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    change = MenuSystem_GetTransitionScale_50D9F0(globalState, nullptr) * change;

    if (CSF_AT(uint8_t, GetGlobalValue_8BC1E0(), 0x90C) &&
        !CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
        change = 1000000.0f;

    const uint32_t actionFlags = self->field_02e0;
    int resourceId;
    if (actionFlags & 0x10000)
    {
        float cost;
        if (self->field_0054 == 3)
        {
            if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                cost = parameters[1568];
            else
                cost = parameters[1557];
        }
        else
        {
            if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                cost = parameters[1567];
            else
                cost = parameters[1556];
        }
        change -= cost * g_flt_0082DC84;

        if (self->field_03d0)
        {
            if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                cost = parameters[1569];
            else
                cost = parameters[1558];
            change -= cost * g_flt_0082DC84;
            resourceId = 16;
        }
        else if (self->field_02e4 & 1)
            resourceId = 15;
        else
            resourceId = 14;
    }
    else if (actionFlags & 0x40)
    {
        void* character = reinterpret_cast<void*>(self->field_0044);
        if (!static_cast<uint8_t>(
                CharActionFsm_GetCharacterToggleState(self, nullptr)))
            goto ApplyChange;

        if (((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, character, 0)[0xF4 / 4])(character, nullptr))
        {
            change = 0.0f;
            goto ApplyChange;
        }

        float cost;
        if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
            cost = parameters[1570];
        else
            cost = parameters[1559];
        change -= cost * g_flt_0082DC84;
        resourceId = 57;
    }
    else
    {
        if (actionFlags & 0x4000)
            goto ApplyChange;

        if (actionFlags & 2)
        {
            float cost;
            if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                cost = parameters[1566];
            else
                cost = parameters[1555];
            change -= cost * g_flt_0082DC84;

            if (!(self->field_02f0 < self->field_02ec))
                goto ApplyChange;
            resourceId = 9;
        }
        else
        {
            if (actionFlags & 1)
            {
                float cost;
                if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                    cost = parameters[1565];
                else
                    cost = parameters[1554];
                change -= cost * g_flt_0082DC84;
            }

            float thresholdPercent;
            if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0))
                thresholdPercent =
                    CSF_AT(float, GetGlobalValue_8BC1E0(), 0x1898);
            else
                thresholdPercent =
                    CSF_AT(float, GetGlobalValue_8BC1E0(), 0x186C);

            resourceId =
                thresholdPercent * self->field_02ec * g_ProgressScale >
                        self->field_02f0
                    ? 10
                    : -1;
        }
    }

    {
        auto* resourceManager =
            reinterpret_cast<C_00833908*>(Get_008b0678());
        C_00833908_SetSceneResourceIfSceneMatches(
            resourceManager, nullptr, reinterpret_cast<int>(self), resourceId);
    }

ApplyChange:
    const float updatedResource = change + self->field_02f0;
    self->field_02f0 = updatedResource;
    if (updatedResource > self->field_02ec)
        self->field_02f0 = self->field_02ec;

    if (self->field_02f0 < g_fZero)
    {
        const uint32_t currentFlags = self->field_02e0;
        if (currentFlags & 0x10000)
        {
            const float accumulatedDeficit =
                self->field_02f4 - self->field_02f0;
            self->field_02f4 = accumulatedDeficit;
            if (!(accumulatedDeficit < g_TransitionOne))
            {
                alignas(4) uint8_t animationBuffer[0x2C];
                ColorAnimBufferInit(animationBuffer, nullptr);

                void* recipient = reinterpret_cast<void*>(self->field_0038);
                const int32_t amount = static_cast<int32_t>(
                    static_cast<int64_t>(self->field_02f4));
                CSF_AT(int32_t, animationBuffer, 0x10) = amount;
                CSF_AT(int32_t, animationBuffer, 0x24) = 3;
                ((void(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, recipient, 0)[0x8C / 4])(
                    recipient, nullptr, animationBuffer);

                self->field_02f0 = 0.0f;
                self->field_02f4 = self->field_02f4 - static_cast<float>(amount);
                return;
            }
        }
        else if (currentFlags & 0x40)
        {
            CharActionFsm_SetCharacterToggleState(self, nullptr, 0);
        }
        self->field_02f0 = 0.0f;
    }
}

HOOK(0x00497930, C_00833ee8_UpdateActionResource, "auto");
