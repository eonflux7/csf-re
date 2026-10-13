#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(void, __fastcall, Rotation_ApplyYawDelta, 0x006AE6B0, (S_006ae570* self, void* , float yawDelta));

CSF_GLOBAL(float, g_RadiansToDegrees, 0x00833AE8);

static std::uint32_t* __fastcall C_00833b88_GetYawRotation(
    C_00833b88* self, void* /*edx*/, std::uint32_t* output)
{
    alignas(8) std::uint8_t rotationBytes[0x8];
    S_006ae570& rotation = *reinterpret_cast<S_006ae570*>(rotationBytes);
    rotation.field_0000 = 0.0f;
    rotation.field_0004 = 0.0f;

    void* source = reinterpret_cast<void*>(self->vGetField016c());
    const std::uint8_t usesMatrix =
        reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0xA8 / 4])(source, nullptr);

    if (usesMatrix != 0)
    {
        source = reinterpret_cast<void*>(self->vGetField016c());
        void* orientation =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);

        alignas(8) std::uint8_t angleBytes[0xC];
        alignas(8) std::uint8_t matrixBytes[0x8];

        float** matrix =
            reinterpret_cast<float**(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, orientation, 0)[0x8C / 4])(
                    orientation, nullptr, matrixBytes);

        float* angles = GfxMatrix_GetEulerAngles(
            matrix, nullptr, reinterpret_cast<float*>(angleBytes));
        const float yaw = angles[1] * g_RadiansToDegrees;
        Rotation_ApplyYawDelta(&rotation, nullptr, yaw);

        if (CSF_AT(std::uint8_t, matrixBytes, 0x4) != 0)
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));
    }
    else
    {
        source = reinterpret_cast<void*>(self->vGetField016c());
        void* orientation =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, source, 0)[0x18 / 4])(source, nullptr);

        const float yaw = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, orientation, 0)[0x28 / 4])(
                    orientation, nullptr));
        Rotation_ApplyYawDelta(&rotation, nullptr, yaw);
    }

    const std::uint32_t first = CSF_AT(std::uint32_t, rotationBytes, 0);
    const std::uint32_t second = CSF_AT(std::uint32_t, rotationBytes, 0x4);
    output[0] = first;
    output[1] = second;
    return output;
}

HOOK(0x00492DE0, C_00833b88_GetYawRotation, "auto");
