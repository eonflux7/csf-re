#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorAddAnimTime, 0x0061F150, (int a1, float a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(void, __fastcall, GfxHAnim_UpdateAnimationChannel, 0x006C33C0, (void* self, void* edx, int a2, float a3, float a4));
CSF_FUNC(uint32_t*, __stdcall, GfxHAnim_ApplyAnimationChannels, 0x006E85B0, (int a1, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, C_00861e98_GetRelativeMatrix, 0x006E8730, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_ExtractRootMotion, 0x006E92A0, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, float* a6, uint32_t* a7, uint32_t* a8));
CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AnimationEndEpsilon, 0x00861E00);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(uint32_t, g_DefaultMatrix, 0x008BBABC);

static void __fastcall C_00861e98_AdvanceAnimationAndRemoveRootMotion(
    C_00861e98* self, void* /*edx*/, void* animation, void* hierarchy,
    float deltaTime)
{
    void* interpolator = CSF_AT(void*, hierarchy, 0x20);
    void* clip = CSF_AT(void*, interpolator, 0x0);
    const float previousTime = CSF_AT(float, interpolator, 0x4);
    const float duration = CSF_AT(float, clip, 0xC);

    RtAnimInterpolatorAddAnimTime(reinterpret_cast<int>(interpolator), deltaTime);
    if (previousTime + deltaTime > duration &&
        (CSF_AT(uint8_t, CSF_AT(void*, animation, 0x4), 0x10) & 1) == 0)
    {
        const float endTime = duration - g_AnimationEndEpsilon;
        RtAnimInterpolatorSetCurrentTime(
            reinterpret_cast<int>(CSF_AT(void*, hierarchy, 0x20)), endTime);
    }

    if ((CSF_AT(uint8_t, animation, 0x14) & 2) != 0 ||
        !CSF_AT(void*, CSF_AT(void*, animation, 0x4), 0x14))
        return;

    auto* channel = reinterpret_cast<uint8_t*>(self) + 0x80;
    for (int remaining = 8; remaining; --remaining, channel += 4)
    {
        const int channelId = CSF_AT(int, channel, 0x0);
        if (channelId)
        {
            // This flag is passed as float bits, not converted numerically.
            const uint32_t flagBits = CSF_AT(uint8_t, animation, 0x14) & 1;
            float channelFlag;
            std::memcpy(&channelFlag, &flagBits, sizeof(channelFlag));
            GfxHAnim_UpdateAnimationChannel(
                CSF_AT(void*, CSF_AT(void*, animation, 0x4), 0x14),
                nullptr, channelId, deltaTime, channelFlag);
        }
    }

    GfxHAnim_ApplyAnimationChannels(
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x80),
        CSF_AT(uint32_t*, CSF_AT(void*, animation, 0x4), 0x14),
        reinterpret_cast<int>(hierarchy));

    const uint32_t flags =
        CSF_AT(uint32_t, CSF_AT(void*, animation, 0x4), 0x10);
    const bool removeTranslation = (flags & 0x40) != 0;
    const bool removeRotation = (flags & 0x80) != 0;
    if ((self->field_0058 & 1) == 0 ||
        (!removeTranslation && !removeRotation))
        return;

    const int targetIndex = static_cast<int32_t>(self->field_0030);
    if (targetIndex < 0 || static_cast<int32_t>(self->field_0034) < 0)
        return;

    void* animationData = CSF_AT(void*, CSF_AT(void*, animation, 0x4), 0x8);
    auto* targetTable = CSF_AT(uint8_t*, animationData, 0x20);
    const uint32_t targetOffset =
        static_cast<uint32_t>(targetIndex) * CSF_AT(uint32_t, targetTable, 0x24);
    auto* targetFrame = targetTable + targetOffset + 0x4C;
    auto* baseFrame = CSF_AT(uint8_t*, animationData, 0x20) + 0x4C;
    auto* sourceTable = CSF_AT(uint8_t*, animationData, 0x20);
    auto* sourceFrame = sourceTable +
        self->field_0034 * CSF_AT(uint32_t, sourceTable, 0x24) + 0x4C;

    // Preserve the contiguous output vector and both eight-byte matrix handles.
    alignas(8) uint8_t scratch[0x2C];
    auto* axis = reinterpret_cast<uint32_t*>(scratch);
    auto* displacement = scratch + 0xC;
    auto* position = scratch + 0x18;
    auto* inverseHandle = scratch + 0x1C;
    auto* matrixHandle = scratch + 0x24;
    float rotation = 0.0f;

    C_00861e98_ExtractRootMotion(
        self, nullptr,
        reinterpret_cast<int>(sourceFrame),
        reinterpret_cast<int>(targetFrame),
        reinterpret_cast<int>(baseFrame),
        reinterpret_cast<uint32_t*>(position), &rotation,
        reinterpret_cast<uint32_t*>(displacement), axis);

    CSF_AT(float, displacement, 0x0) =
        CSF_AT(float, position, 0x0) - CSF_AT(float, self, 0x24);
    CSF_AT(float, displacement, 0x4) =
        CSF_AT(float, position, 0x4) - CSF_AT(float, self, 0x28);
    CSF_AT(float, displacement, 0x8) =
        CSF_AT(float, position, 0x8) - CSF_AT(float, self, 0x2C);

    if (removeRotation && rotation != g_fZero)
    {
        CSF_AT(int, matrixHandle, 0x0) =
            C_00861e98_GetRelativeMatrix(
                self, nullptr, reinterpret_cast<int>(targetFrame),
                reinterpret_cast<int>(baseFrame));
        CSF_AT(uint8_t, matrixHandle, 0x4) = 0;
        CSF_AT(uint32_t, inverseHandle, 0x0) = g_DefaultMatrix;
        CSF_AT(uint8_t, inverseHandle, 0x4) = 0;

        // The packet labels this call address as a vtable reference.
        (void)CSF_VTBL(0x006ADB40);
        Matrix_InvertWithStoredTarget(
            reinterpret_cast<S_006adb40*>(matrixHandle), nullptr,
            reinterpret_cast<float**>(inverseHandle));
        S_00526d40_TransformVector3(
            reinterpret_cast<S_00526d40*>(&g_AxisY), nullptr,
            axis, reinterpret_cast<int*>(inverseHandle));
        RtQuatRotate(
            reinterpret_cast<int>(targetFrame + 0x8),
            reinterpret_cast<float*>(axis), -rotation, 1);

        if (CSF_AT(uint8_t, inverseHandle, 0x4))
            RwMatrixDestroy(CSF_AT(uint32_t*, inverseHandle, 0x0));
        if (CSF_AT(uint8_t, matrixHandle, 0x4))
            RwMatrixDestroy(CSF_AT(uint32_t*, matrixHandle, 0x0));
    }

    if (removeTranslation)
    {
        CSF_AT(float, targetFrame, 0x18) -= CSF_AT(float, displacement, 0x0);
        CSF_AT(float, targetFrame, 0x1C) -= CSF_AT(float, displacement, 0x4);
        CSF_AT(float, targetFrame, 0x20) -= CSF_AT(float, displacement, 0x8);
    }
}

HOOK(0x006EAF30, C_00861e98_AdvanceAnimationAndRemoveRootMotion, "auto");
