#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(int, __fastcall, C_00861e98_GetFrameTransform, 0x006E8610, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_GetRootMatrix, 0x006E8730, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_CalculateRootMotion, 0x006E92A0, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, float* a6, uint32_t* a7, uint32_t* a8));
CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(uint32_t, g_DefaultMatrixTarget, 0x008BBABC);

// The packet also identifies the inversion entry as a vtable reference.
static const auto g_MatrixInversionReference = CSF_VTBL(0x006ADB40);

static char __fastcall C_00861e98_ExtractRootMotion(
    C_00861e98* self, void* /*edx*/, char resetPose,
    char extractTranslation, char extractRotation, uint32_t* recipient)
{
    char result = resetPose;
    if (resetPose)
    {
        void* pose = CSF_AT(void*, self, 0x1C);
        result = static_cast<char>(reinterpret_cast<std::uintptr_t>(pose));
        std::memcpy(
            CSF_AT(void*, self, 0x48),
            static_cast<uint8_t*>(CSF_AT(void*, pose, 0x4)) + 0x10,
            0x40);
    }

    alignas(8) uint8_t axisBytes[0xC]{};
    alignas(8) uint8_t translationBytes[0xC]{};
    alignas(8) uint8_t appliedBytes[0xC]{};
    alignas(8) uint8_t rotationBytes[0x18]{};

    // The position output overlaps the two temporary matrix holders.
    alignas(8) uint8_t positionAndMatricesBytes[0x14]{};
    auto* position = reinterpret_cast<uint32_t*>(positionAndMatricesBytes);
    auto* translation = reinterpret_cast<uint32_t*>(translationBytes);
    auto* applied = reinterpret_cast<uint32_t*>(appliedBytes);
    auto* rotation = reinterpret_cast<uint32_t*>(rotationBytes);
    auto* axis = reinterpret_cast<uint32_t*>(axisBytes);

    if ((CSF_AT(uint8_t, self, 0x58) & 1) != 0)
    {
        int sourceIndex = CSF_AT(int, self, 0x30);
        if (sourceIndex < 0)
            return result;

        int targetIndex = CSF_AT(int, self, 0x34);
        if (targetIndex < 0)
            return result;

        void* animation = CSF_AT(void*, self, 0x14);
        auto* hierarchy = CSF_AT(uint8_t*, animation, 0x20);
        uint32_t stride = CSF_AT(uint32_t, hierarchy, 0x24);
        auto* sourceFrame = hierarchy + 0x4C + uint32_t(sourceIndex) * stride;
        auto* frames = hierarchy + 0x4C;
        auto* targetFrame = hierarchy + 0x4C
            + uint32_t(targetIndex) * CSF_AT(uint32_t, hierarchy, 0x24);

        float angle = 0.0f;
        C_00861e98_CalculateRootMotion(
            self, nullptr, reinterpret_cast<int>(targetFrame),
            reinterpret_cast<int>(sourceFrame), reinterpret_cast<int>(frames),
            position, &angle, translation, rotation);

        float deltaX = CSF_AT(float, positionAndMatricesBytes, 0x0)
            - CSF_AT(float, self, 0x24);
        float deltaY = CSF_AT(float, positionAndMatricesBytes, 0x4)
            - CSF_AT(float, self, 0x28);
        float deltaZ = CSF_AT(float, positionAndMatricesBytes, 0x8)
            - CSF_AT(float, self, 0x2C);

        if (extractRotation && angle != g_fZero)
        {
            S_006adb40& sourceMatrix =
                *reinterpret_cast<S_006adb40*>(positionAndMatricesBytes + 0x4);
            auto* targetMatrixBytes = positionAndMatricesBytes + 0xC;

            sourceMatrix.field_0000 = C_00861e98_GetRootMatrix(
                self, nullptr, reinterpret_cast<int>(sourceFrame),
                reinterpret_cast<int>(frames));
            CSF_AT(uint8_t, &sourceMatrix, 0x4) = 0;
            std::memcpy(&sourceMatrix.field_0008, &g_DefaultMatrixTarget, 4);
            CSF_AT(uint8_t, targetMatrixBytes, 0x4) = 0;

            Matrix_InvertWithStoredTarget(
                &sourceMatrix, nullptr,
                reinterpret_cast<float**>(&sourceMatrix.field_0008));
            S_00526d40_TransformVector3(
                reinterpret_cast<S_00526d40*>(&g_AxisY), nullptr,
                axis, reinterpret_cast<int*>(targetMatrixBytes));
            RtQuatRotate(
                reinterpret_cast<int>(sourceFrame + 0x8),
                reinterpret_cast<float*>(axis), -angle, 1);

            applied[1] = rotation[1];
            applied[0] = rotation[0];
            applied[2] = rotation[2];
            ((void(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, recipient, 0)[0x58 / 4])(
                    recipient, nullptr, applied);

            if (CSF_AT(uint8_t, targetMatrixBytes, 0x4))
            {
                RwMatrixDestroy(
                    CSF_AT(uint32_t*, targetMatrixBytes, 0x0));
            }
            if (CSF_AT(uint8_t, &sourceMatrix, 0x4))
            {
                RwMatrixDestroy(
                    reinterpret_cast<uint32_t*>(sourceMatrix.field_0000));
            }
        }

        result = extractTranslation;
        if (extractTranslation)
        {
            CSF_AT(float, sourceFrame, 0x18) -= deltaX;
            applied[2] = translation[2];
            applied[0] = translation[0];
            applied[1] = translation[1];
            CSF_AT(float, sourceFrame, 0x1C) -= deltaY;
            CSF_AT(float, sourceFrame, 0x20) -= deltaZ;

            result = ((char(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, recipient, 0)[0x1C / 4])(
                    recipient, nullptr, applied);
        }
    }
    else
    {
        int targetIndex = CSF_AT(int, self, 0x34);
        if (targetIndex < 0)
            return result;

        void* animation = CSF_AT(void*, self, 0x14);
        auto* hierarchy = CSF_AT(uint8_t*, animation, 0x20);
        auto* frame = hierarchy + 0x4C
            + uint32_t(targetIndex) * CSF_AT(uint32_t, hierarchy, 0x24);
        auto* transform = reinterpret_cast<uint8_t*>(
            C_00861e98_GetFrameTransform(
                self, nullptr, reinterpret_cast<int>(frame)));

        if (extractRotation)
        {
            CSF_AT(uint32_t, frame, 0x8) = CSF_AT(uint32_t, self, 0x38);
            CSF_AT(uint32_t, frame, 0xC) = CSF_AT(uint32_t, self, 0x3C);
            CSF_AT(uint32_t, frame, 0x10) = CSF_AT(uint32_t, self, 0x40);
            CSF_AT(uint32_t, frame, 0x14) = CSF_AT(uint32_t, self, 0x44);

            translation[0] = CSF_AT(uint32_t, transform, 0x20);
            translation[1] = CSF_AT(uint32_t, transform, 0x24);
            translation[2] = CSF_AT(uint32_t, transform, 0x28);
            ((void(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, recipient, 0)[0x58 / 4])(
                    recipient, nullptr, translation);
        }

        result = extractTranslation;
        if (extractTranslation)
        {
            CSF_AT(uint32_t, frame, 0x18) = 0;
            CSF_AT(uint32_t, frame, 0x1C) = 0;
            CSF_AT(uint32_t, frame, 0x20) = 0;

            translation[0] = CSF_AT(uint32_t, transform, 0x30);
            translation[1] = CSF_AT(uint32_t, transform, 0x34);
            translation[2] = CSF_AT(uint32_t, transform, 0x38);
            result = ((char(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, recipient, 0)[0x1C / 4])(
                    recipient, nullptr, translation);
        }
    }
    return result;
}

HOOK(0x006EAB60, C_00861e98_ExtractRootMotion, "auto");
