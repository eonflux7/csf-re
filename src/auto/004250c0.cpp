#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// --- callees (bound by address) ---------------------------------------------------------------
// Forwarder to the child task's update: takes what this function pushes (the frame delta time).
CSF_FUNC(void, __fastcall, TCharTask_ForwardUpdateToChild, 0x004DB0F0, (void* self, void* edx, float deltaTime));
CSF_FUNC(int, __fastcall, AiSubStateObject_Advance, 0x006AF1C0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, SetAngleFieldsFromNormalizedAngle, 0x0041E420, (void* self, void* , float angle));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __fastcall, AiTaskState_UpdateNormalizedAngle, 0x0041E7E0, (void* self, void* , float angle));
CSF_FUNC(int, __fastcall, AiTaskStates_FieldsEqual, 0x0041E460, (float* self, void*));
CSF_FUNC(double, __fastcall, AiTaskStates_AdvanceAngleTween, 0x0041E490, (float* self, void* , float a2));
// Forwarder: receives the heading in degrees pushed by this function.
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float headingDegrees));
CSF_FUNC(int, __fastcall, AiSubStateObject_TestReady, 0x006AEEA0, (void* self, void* edx));

// --- constants (bound by address) -------------------------------------------------------------
CSF_GLOBAL(float, g_RadToDeg_82FCAC, 0x0082FCAC);          // 57.29578
CSF_GLOBAL(float, g_DegToRad_82FCB0, 0x0082FCB0);          // 0.017453292
CSF_GLOBAL(float, g_MinMoveDistSq_82FCB4, 0x0082FCB4);     // 0.000001

// Per-frame update of the "follow a path / mover" char task: advances the mover, places the
// character on the mover's current position, turns it smoothly towards the movement direction
// and, when the mover is done, finishes the task.
static void __fastcall C_0082fcd8_UpdateMoveAlongPath(C_0082fcd8* self, void* /*edx*/, float deltaTime)
{
    TCharTask_ForwardUpdateToChild(self, nullptr, deltaTime);

    if (self->field_0020 != 1)
        return;

    AiSubStateObject_Advance(reinterpret_cast<void*>(self->field_0044 + 4), nullptr, deltaTime);

    float currentPos[3];
    float moveDelta[3];

    const uint8_t* moverPos = reinterpret_cast<const uint8_t*>(self->field_0044) + 0x5C;
    currentPos[0] = CSF_AT(float, moverPos, 0x0);
    moveDelta[0] = currentPos[0] - self->field_0038;
    currentPos[1] = CSF_AT(float, moverPos, 0x4);
    currentPos[2] = CSF_AT(float, moverPos, 0x8);
    moveDelta[1] = currentPos[1] - self->field_003c;
    moveDelta[2] = currentPos[2] - self->field_0040;

    SceneLoader_ConfigureFocusEntry(reinterpret_cast<void*>(self->field_0014), nullptr,
                                    reinterpret_cast<int*>(currentPos));

    uint8_t* angleTween = reinterpret_cast<uint8_t*>(self) + 0x48;

    const float moveDistSq = moveDelta[2] * moveDelta[2] + moveDelta[1] * moveDelta[1] + moveDelta[0] * moveDelta[0];
    if (moveDistSq > g_MinMoveDistSq_82FCB4)
    {
        const float currentHeading = static_cast<float>(
            C_008342e8_ComputeResolvedObjectScalar(reinterpret_cast<C_008342e8*>(self->field_0014), nullptr)
            * g_DegToRad_82FCB0);
        SetAngleFieldsFromNormalizedAngle(angleTween, nullptr, currentHeading);

        const float targetHeading = static_cast<float>(
            S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(moveDelta), nullptr));
        AiTaskState_UpdateNormalizedAngle(angleTween, nullptr, targetHeading);

        self->field_0058 = 0;
        self->field_005c = 0x3DCCCCCD; // 0.1f
    }

    if (!static_cast<uint8_t>(AiTaskStates_FieldsEqual(reinterpret_cast<float*>(angleTween), nullptr)))
    {
        const float headingDegrees = static_cast<float>(
            AiTaskStates_AdvanceAngleTween(reinterpret_cast<float*>(angleTween), nullptr, deltaTime)
            * g_RadToDeg_82FCAC);
        DispatchSceneObjectWithPayload(reinterpret_cast<void*>(self->field_0014), nullptr, headingDegrees);
    }

    self->field_0038 = currentPos[0];
    self->field_003c = currentPos[1];
    self->field_0040 = currentPos[2];

    if (static_cast<uint8_t>(AiSubStateObject_TestReady(reinterpret_cast<void*>(self->field_0044 + 4), nullptr)))
    {
        void* ownerSubObject = reinterpret_cast<uint8_t*>(self->field_0014) + 4;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, ownerSubObject, 0)[4])(ownerSubObject, nullptr);

        self->field_0018 |= 4u;

        const uint32_t mover = self->field_0044;
        self->field_0020 = 0;
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, self, 0)[42])(self, nullptr, mover);
    }
}

HOOK(0x004250C0, C_0082fcd8_UpdateMoveAlongPath, "auto");
