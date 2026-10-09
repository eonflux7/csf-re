#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_00833b88_ApplyYawInput, 0x004983C0, (void* self, void* edx, float a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(uint32_t*, __fastcall, RotateMatrixAroundY, 0x006AE1D0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, AddPitchAngle, 0x006AE690, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00833DA0);
CSF_GLOBAL(float, g_MinWidePitch, 0x00833E38);
CSF_GLOBAL(float, g_MaxWidePitch, 0x00833E3C);
CSF_GLOBAL(float, g_MinNarrowPitch, 0x00833E40);

static char __fastcall C_00833b88_UpdateLookRotation(
    C_00833b88* self, void* /*edx*/, float* input)
{
    auto* pitchState = reinterpret_cast<std::uint8_t*>(self) + 0x1AC;
    float pitchDelta = CSF_AT(float, self, 0x390) * input[0];
    float yawDelta = input[1] * CSF_AT(float, self, 0x390);

    C_00833b88_ApplyYawInput(self, nullptr, yawDelta);

    if ((CSF_AT(std::uint8_t, self, 0x2E0) & 0x40) != 0)
    {
        void* owner = reinterpret_cast<void*>(self->field_0044);
        auto* subObject =
            reinterpret_cast<C_008319c0* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0xD8 / 4])(owner, nullptr);

        if (subObject &&
            C_008319c0_GetSubObjectField74(subObject, nullptr) == 2)
        {
            if (CSF_AT(std::uint8_t,
                       reinterpret_cast<void*>(GetGlobalState_50D190()),
                       0xD0))
            {
                pitchDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1B8);
                yawDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1B8);
            }
            else
            {
                pitchDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1B4);
                yawDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1B4);
            }

            if (!CSF_AT(std::uint8_t, self, 0x360))
            {
                pitchDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x18A8);
                yawDelta *= CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x18A8);
            }
        }
    }

    AddPitchAngle(pitchState, nullptr, pitchDelta);

    if (CSF_AT(std::int32_t, self, 0x2DC) < 0)
    {
        if (CSF_AT(float, pitchState, 0x4) > g_MaxWidePitch)
            CSF_AT(float, pitchState, 0x4) = 80.0f;
        if (CSF_AT(float, pitchState, 0x4) < g_MinWidePitch)
            CSF_AT(float, pitchState, 0x4) = -80.0f;
    }
    else
    {
        if (CSF_AT(float, pitchState, 0x4) > g_QuarterTurnConstant)
            CSF_AT(float, pitchState, 0x4) = 30.0f;
        if (CSF_AT(float, pitchState, 0x4) < g_MinNarrowPitch)
            CSF_AT(float, pitchState, 0x4) = -30.0f;
    }

    if (CSF_AT(std::uint8_t, self, 0x228))
    {
        void* object = reinterpret_cast<void*>(self->vGetField016c());
        void* transform =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

        return reinterpret_cast<char (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, transform, 0)[0x40 / 4])(
                transform, nullptr, yawDelta);
    }

    void* source = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));

    std::uint32_t sourceVector[3];
    sourceVector[0] = CSF_AT(std::uint32_t, source, 0x18);
    sourceVector[1] = CSF_AT(std::uint32_t, source, 0x1C);
    sourceVector[2] = CSF_AT(std::uint32_t, source, 0x20);

    void* object = reinterpret_cast<void*>(self->vGetField016c());
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    float temporaryVector[3] = {};
    float* transformedVector =
        reinterpret_cast<float* (__fastcall*)(
            void*, void*, float*, std::uint32_t*)>(
                CSF_AT(void**, transform, 0)[0xA4 / 4])(
                    transform, nullptr, temporaryVector, sourceVector);

    float* translation =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    float pivot[3];
    pivot[2] = transformedVector[2] + translation[2];
    pivot[1] = transformedVector[1] + translation[1];
    pivot[0] = transformedVector[0] + translation[0];

    temporaryVector[0] = -pivot[0];
    temporaryVector[1] = -pivot[1];
    temporaryVector[2] = -pivot[2];

    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, transform, 0)[0x20 / 4])(
            transform, nullptr, temporaryVector);

    // The matrix handle and ownership flag occupy the same eight-byte
    // stack object; its second word initially contains the pitch delta.
    std::uint32_t matrixState[2] = {};
    std::memcpy(&matrixState[1], &pitchDelta, sizeof(pitchDelta));

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, transform, 0)[0x8C / 4])(
            transform, nullptr, matrixState);

    RotateMatrixAroundY(matrixState, nullptr, yawDelta * g_DegreesToRadians);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, transform, 0)[0x94 / 4])(
            transform, nullptr, matrixState);

    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, transform, 0)[0x20 / 4])(
            transform, nullptr, pivot);

    char result = reinterpret_cast<const char*>(matrixState)[4];
    if (result)
        return static_cast<char>(
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixState[0])));
    return result;
}

HOOK(0x00498A40, C_00833b88_UpdateLookRotation, "auto");
