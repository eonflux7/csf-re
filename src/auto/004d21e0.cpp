#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __fastcall, NetCogible_ReadAngleDegreesMod360, 0x00740AF0, (void* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall C_0083ba40_ApplyClampedJointAngle(
    C_0083ba40* self, void* /*edx*/, float angle)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* character =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* joints =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, character, 0)[0x68 / 4])(character, nullptr);

    void* firstJoint =
        reinterpret_cast<void*(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, joints, 0)[0x6C / 4])(
                joints, nullptr, self->field_00a8);
    void* secondJoint =
        reinterpret_cast<void*(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, joints, 0)[0x6C / 4])(
                joints, nullptr, self->field_00ac);

    // The float parameter's bits are used as the angle-source pointer.
    std::uint32_t angleSource;
    std::memcpy(&angleSource, &angle, sizeof(angleSource));
    angle = static_cast<float>(NetCogible_ReadAngleDegreesMod360(
        reinterpret_cast<void*>(angleSource), nullptr));

    // The matrix output and its ownership byte share the original bounds storage.
    alignas(8) std::uint8_t scratchBytes[0x10] = {};
    float* firstBound = reinterpret_cast<float*>(scratchBytes + 0x4);
    float* secondBound = reinterpret_cast<float*>(scratchBytes + 0x8);

    if (self->field_0042)
    {
        *secondBound = 30.0f;
        *firstBound = -50.0f;
        ClampFloatToBounds(&angle, firstBound, secondBound);
    }
    else
    {
        *firstBound = 50.0f;
        *secondBound = -50.0f;
        ClampFloatToBounds(&angle, secondBound, firstBound);
    }

    if (firstJoint && secondJoint)
    {
        void* matrixResult =
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, firstJoint, 0)[0x14 / 4])(
                    firstJoint, nullptr, scratchBytes);

        angle *= g_HalfDurationScale;
        std::uint8_t* axis = CSF_AT(std::uint8_t*, matrixResult, 0) + 0x20;
        const float halfAngle = angle;

        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*, float)>(
            CSF_AT(void**, joints, 0)[0x78 / 4])(
                joints, nullptr, self->field_00a8, axis, halfAngle);

        if (scratchBytes[0x4])
            RwMatrixDestroy(CSF_AT(std::uint32_t*, scratchBytes, 0));

        matrixResult =
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, secondJoint, 0)[0x14 / 4])(
                    secondJoint, nullptr, scratchBytes);

        axis = CSF_AT(std::uint8_t*, matrixResult, 0) + 0x20;
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*, float)>(
            CSF_AT(void**, joints, 0)[0x78 / 4])(
                joints, nullptr, self->field_00ac, axis, halfAngle);

        if (scratchBytes[0x4])
            RwMatrixDestroy(CSF_AT(std::uint32_t*, scratchBytes, 0));
    }
}

HOOK(0x004D21E0, C_0083ba40_ApplyClampedJointAngle, "auto");
