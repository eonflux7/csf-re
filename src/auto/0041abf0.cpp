#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_MaxVehicleTimeStep, 0x0082E26C);
CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

CSF_FUNC(int, __fastcall, C_0082e1e8_ApplyMotionInput, 0x00418F40, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_0082e1e8_ApplyMotionResponse, 0x004190C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082e1e8_FinalizeMotion, 0x004194F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082e1e8_RefreshMotionState, 0x00419810, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082e1e8_ApplyMotionConstraints, 0x00419B50, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, C_0082e1e8_IntegrateMotion, 0x00419FC0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082e1e8_AdjustIntegratedMotion, 0x0041A160, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082e1e8_ResolveMotion, 0x0041A2F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082e1e8_PrepareMotionConstraints, 0x0041A530, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static float Vehicle_FloatFromBits(std::uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static char __fastcall C_0082e1e8_UpdateMotion(
    C_0082e1e8* self, void* /*edx*/, float elapsedTime)
{
    char result = static_cast<char>(self->field_0085);
    if (!result)
        return result;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* returnedObject =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4]))(owner, nullptr);
    void* body =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, returnedObject, 0)[0x18 / 4]))(
                returnedObject, nullptr);

    // These buffers retain the original overlap between scratch objects
    // and the saved position and scalar.
    std::uint32_t motion[0x5C / 4]{};
    std::uint32_t matrixState[0x18 / 4]{};
    float vectors[0x18 / 4]{};

    CSF_AT(float, matrixState, 0x14) = static_cast<float>(
        (reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, body, 0)[0x28 / 4]))(body, nullptr));

    float* position =
        (reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, body, 0)[0x10 / 4]))(body, nullptr);
    CSF_AT(float, matrixState, 0x08) = position[0];
    CSF_AT(float, matrixState, 0x0C) = position[1];
    CSF_AT(float, matrixState, 0x10) = position[2];
    motion[19] = 0;
    motion[20] = 0;
    motion[21] = 0;
    motion[22] = 10;
    CSF_AT(float, motion, 0) =
        elapsedTime <= g_MaxVehicleTimeStep ? elapsedTime : 0.059999999f;

    void* motionObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));

    void* matrixHandle =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, body, 0)[0x8C / 4]))(
                body, nullptr, matrixState);
    float* direction = reinterpret_cast<float*>(
        reinterpret_cast<std::uint8_t*>(CSF_AT(void*, matrixHandle, 0)) + 0x10);

    void* motionParameters =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, motionObject, 0)[0x30 / 4]))(
                motionObject, nullptr);
    float scale = CSF_AT(float, motionParameters, 0x04) *
                  g_SoundActivityThreshold;
    vectors[0] = scale * direction[0];
    vectors[1] = scale * direction[1];
    vectors[2] = scale * direction[2];

    position =
        (reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, body, 0)[0x10 / 4]))(body, nullptr);
    vectors[3] = vectors[0] + position[0];
    CSF_AT(float, motion, 0x38) = vectors[3];
    vectors[4] = vectors[1] + position[1];
    CSF_AT(float, motion, 0x3C) = vectors[4];
    vectors[5] = vectors[2] + position[2];
    CSF_AT(float, motion, 0x40) = vectors[5];

    if (CSF_AT(std::uint8_t, matrixState, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixState, 0));

    C_0082e1e8_RefreshMotionState(self, nullptr);
    C_0082e1e8_ApplyMotionInput(self, nullptr, motion);
    C_0082e1e8_ApplyMotionResponse(self, nullptr, reinterpret_cast<int>(motion));
    C_0082e1e8_PrepareMotionConstraints(self, nullptr, reinterpret_cast<int>(motion));
    C_0082e1e8_ApplyMotionConstraints(self, nullptr, motion);
    C_0082e1e8_IntegrateMotion(self, nullptr, reinterpret_cast<int>(motion));
    C_0082e1e8_AdjustIntegratedMotion(self, nullptr, reinterpret_cast<int>(motion));
    C_0082e1e8_ResolveMotion(self, nullptr, reinterpret_cast<int>(motion));
    C_0082e1e8_FinalizeMotion(self, nullptr, reinterpret_cast<int>(motion));

    double currentScalar =
        (reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, body, 0)[0x28 / 4]))(body, nullptr);
    if (CSF_AT(float, matrixState, 0x14) != currentScalar)
    {
        float difference = static_cast<float>(
            CSF_AT(float, matrixState, 0x14) - currentScalar);
        (reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, body, 0)[0x40 / 4]))(body, nullptr, difference);
    }

    position =
        (reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, body, 0)[0x10 / 4]))(body, nullptr);
    vectors[3] = position[0];
    vectors[4] = position[1];
    vectors[5] = position[2];

    if (Vehicle_FloatFromBits(self->field_0020) == g_ZeroVector2X &&
        Vehicle_FloatFromBits(self->field_0024) == g_ZeroVector2Y &&
        (vectors[3] != CSF_AT(float, matrixState, 0x08) ||
         vectors[5] != CSF_AT(float, matrixState, 0x10)))
    {
        vectors[2] = CSF_AT(float, matrixState, 0x10);
        vectors[0] = CSF_AT(float, matrixState, 0x08);
        vectors[1] = vectors[4];
        (reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, body, 0)[0x14 / 4]))(body, nullptr, vectors);
    }

    C_0082e1e8_RefreshMotionState(self, nullptr);
    self->field_0085 =
        self->field_0064 != g_fZero ||
        self->field_0068 != g_fZero ||
        self->field_006c != g_fZero ||
        self->field_0070 != g_fZero ||
        self->field_0074 != g_fZero ||
        self->field_0078 != g_fZero ||
        self->field_0086;

    result = static_cast<char>(motion[19]);
    if (motion[19])
        return static_cast<char>(Mem_Free(reinterpret_cast<void*>(motion[19])));
    return result;
}

HOOK(0x0041ABF0, C_0082e1e8_UpdateMotion, "auto");
