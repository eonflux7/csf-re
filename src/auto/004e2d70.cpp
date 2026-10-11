#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (float* self, void* , float* other));
CSF_FUNC(char*, __fastcall, GfxMatrix_GetVectorData, 0x0040FC30, (void* self, void* edx));
CSF_FUNC(float, __fastcall, SceneObj_GetQueryRate, 0x0048E4F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static char __fastcall C_0083e248_ApplyHorizontalQueryVelocity(
    C_0083e248* self, void* /*edx*/)
{
    void* sceneObject = reinterpret_cast<void*>(self->field_000c);
    void* controller =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);
    void* movement =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x78 / 4])(controller, nullptr);

    if (!movement)
        return 0;

    if (static_cast<uint8_t>(IsField1BCNonNegative(
            reinterpret_cast<void*>(self->field_000c), nullptr)))
        return 0;

    sceneObject = reinterpret_cast<void*>(self->field_000c);
    controller =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);
    void* currentMovement =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x78 / 4])(controller, nullptr);

    if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, currentMovement, 0)[0x2C / 4])(
            currentMovement, nullptr))
        return 0;

    sceneObject = reinterpret_cast<void*>(self->field_000c);
    controller =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneObject, 0)[0x28 / 4])(sceneObject, nullptr);
    currentMovement =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x78 / 4])(controller, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, currentMovement, 0)[0x20 / 4])(
        currentMovement, nullptr);

    alignas(8) uint8_t directionBytes[0x10];
    alignas(8) uint8_t velocityBytes[0xC];
    float* direction = reinterpret_cast<float*>(directionBytes);
    float* velocity = reinterpret_cast<float*>(velocityBytes);
    float& queryRate = CSF_AT(float, directionBytes, 0xC);

    queryRate = SceneObj_GetQueryRate(
        reinterpret_cast<void*>(self->field_000c), nullptr);

    const float* source = reinterpret_cast<const float*>(
        GfxMatrix_GetVectorData(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), 0x4C),
            nullptr));
    direction[0] = source[0];
    direction[1] = source[1];
    direction[2] = source[2];
    direction[1] = 0.0f;

    if (queryRate > g_fZero)
    {
        if (static_cast<uint8_t>(
                GfxMatrix_AreVectorsDifferent(direction, nullptr, &g_ZeroVectorX)))
        {
            NormalizeVector3(
                reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

            velocity[0] = direction[0] * queryRate;
            velocity[1] = direction[1] * queryRate;
            velocity[2] = direction[2] * queryRate;

            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, movement, 0)[0x4C / 4])(
                movement, nullptr, velocity);
        }
    }

    return 1;
}

HOOK(0x004E2D70, C_0083e248_ApplyHorizontalQueryVelocity, "auto");
