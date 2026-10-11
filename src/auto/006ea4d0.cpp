#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtAnimAnimationGetNumNodes, 0x0061EFB0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorCopy, 0x0061F120, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));
CSF_FUNC(int, __fastcall, GfxHAnim_IsSpecialAnimation, 0x006C2060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxHAnim_BlendHierarchyPose, 0x006E8150, (int a1, int a2, int a3));
CSF_FUNC(void, __stdcall, GfxHAnim_SetRootMotionTime, 0x006E8540, (int a1, void* a2, float a3));
CSF_FUNC(uint32_t*, __stdcall, GfxHAnim_ApplyRootMotion, 0x006E85B0, (int a1, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, C_00861e98_UpdateAnimationState, 0x006E8BF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_PrepareBlend, 0x006E8D80, (void* self, void* edx, int* a2));
CSF_FUNC(int, __stdcall, GfxHAnim_ResetTrackBlend, 0x006E8DF0, (uint32_t* a1));
CSF_FUNC(int, __fastcall, C_00861e98_GetAnimationTrack, 0x006E9030, (void* self, void* edx, int a2));
CSF_FUNC(uint8_t, __fastcall, C_00861e98_PrepareBlendRootMotion, 0x006E9080, (C_00861e98* self, void* edx, uint32_t* track, int animation));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_CalculateRootMotion, 0x006E92A0, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5, float* a6, uint32_t* a7, uint32_t* a8));
CSF_FUNC(int, __fastcall, C_00861e98_UpdateHierarchyPose, 0x006E9BF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_PreparePrimaryTrack, 0x006E9D60, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static uint32_t* GfxHAnim_GetHierarchyInterpolator(int hierarchy)
{
    return CSF_AT(uint32_t*, reinterpret_cast<void*>(hierarchy), 0x20);
}

static int GfxHAnim_GetBlendSource(int* track)
{
    return CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x08);
}

static int GfxHAnim_GetBlendDestination(int* track)
{
    return CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x0C);
}

static char __fastcall C_00861e98_StartAnimation(
    C_00861e98* self, void* /*edx*/, int trackIndex,
    float** animationInfo, void* options)
{
    if ((CSF_AT(uint32_t, self, 0x58) & 4) != 0 ||
        !CSF_AT(uint32_t, self, 0x1C))
        return 0;

    uint32_t* animation = nullptr;
    if (animationInfo)
        animation = reinterpret_cast<uint32_t*>(*animationInfo);
    if (!animation)
    {
        animation = CSF_AT(uint32_t*, self, 0x20);
        if (!animation)
            return 0;
    }

    float startTime = CSF_AT(float, options, 0x00);
    if (startTime < g_fZero || startTime > CSF_AT(float, animation, 0x0C))
        startTime = 0.0f;

    int* track = reinterpret_cast<int*>(
        C_00861e98_GetAnimationTrack(self, nullptr, trackIndex));
    int nodeCount = RtAnimAnimationGetNumNodes(animation);
    if (nodeCount != CSF_AT(int, reinterpret_cast<void*>(track[0]), 0x04))
    {
        RtAnimAnimationGetNumNodes(animation);
        return 0;
    }

    float blendDuration = CSF_AT(float, options, 0x04);
    if (((CSF_AT(uint32_t, self, 0x58) & 2) == 0 &&
         !CSF_AT(uint8_t, options, 0x1D)) ||
        ((track[5] & 4) == 0 && (track[5] & 2) == 0) ||
        blendDuration <= g_fZero)
    {
        track[2] = 0;
        track[3] = 0;
        GfxHAnim_ResetTrackBlend(reinterpret_cast<uint32_t*>(track));
        RtAnimInterpolatorSetCurrentAnim(
            GfxHAnim_GetHierarchyInterpolator(track[0]), animation);
        RtAnimInterpolatorSetCurrentTime(
            reinterpret_cast<int>(GfxHAnim_GetHierarchyInterpolator(track[0])),
            startTime);

        if (!trackIndex && animationInfo &&
            !static_cast<uint8_t>(
                GfxHAnim_IsSpecialAnimation(animationInfo, nullptr)))
        {
            GfxHAnim_SetRootMotionTime(
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                animationInfo, startTime);
            GfxHAnim_ApplyRootMotion(
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                reinterpret_cast<uint32_t*>(animationInfo), track[0]);
        }
        C_00861e98_UpdateHierarchyPose(self, nullptr, track[0]);
    }
    else if ((track[5] & 4) != 0)
    {
        C_00861e98_PrepareBlend(self, nullptr, track);
        CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x10) = track[5];

        int previousAnimationInfo = track[7];
        if (!previousAnimationInfo ||
            (CSF_AT(float, track, 0x08) > g_fZero &&
             CSF_AT(float, track, 0x0C) >= g_fZero) ||
            trackIndex)
            track[5] |= 0x400;
        else
            track[5] &= 0xFFFFFBFFu;

        CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x14) =
            previousAnimationInfo;
        int flags = track[5];
        CSF_AT(float, track, 0x08) = blendDuration;
        track[3] = 0;

        if ((flags & 2) != 0)
        {
            int sourceInterpolator = reinterpret_cast<int>(
                GfxHAnim_GetHierarchyInterpolator(track[0]));
            uint32_t* destinationInterpolator =
                GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendSource(track));
            RtAnimInterpolatorCopy(destinationInterpolator, sourceInterpolator);
            track[5] &= 0xFFFFFFE7u;
        }
        else if ((flags & 0x400) != 0)
        {
            int sourceInterpolator = reinterpret_cast<int>(
                GfxHAnim_GetHierarchyInterpolator(track[0]));
            uint32_t* destinationInterpolator =
                GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendSource(track));
            RtAnimInterpolatorCopy(destinationInterpolator, sourceInterpolator);
        }
        else
        {
            uint32_t* previousAnimation =
                CSF_AT(uint32_t*, reinterpret_cast<void*>(track[7]), 0x00);
            RtAnimInterpolatorSetCurrentAnim(
                GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendSource(track)),
                previousAnimation);

            float previousTime = CSF_AT(
                float, GfxHAnim_GetHierarchyInterpolator(track[0]), 0x04);
            RtAnimInterpolatorSetCurrentTime(
                reinterpret_cast<int>(GfxHAnim_GetHierarchyInterpolator(
                    GfxHAnim_GetBlendSource(track))),
                previousTime);

            if (!trackIndex)
            {
                GfxHAnim_ApplyRootMotion(
                    reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                    reinterpret_cast<uint32_t*>(animationInfo),
                    GfxHAnim_GetBlendSource(track));
            }
            C_00861e98_UpdateHierarchyPose(
                self, nullptr, GfxHAnim_GetBlendSource(track));
        }

        RtAnimInterpolatorSetCurrentAnim(
            GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendDestination(track)),
            animation);
        RtAnimInterpolatorSetCurrentTime(
            reinterpret_cast<int>(GfxHAnim_GetHierarchyInterpolator(
                GfxHAnim_GetBlendDestination(track))),
            startTime);

        if (!trackIndex && animationInfo &&
            !static_cast<uint8_t>(
                GfxHAnim_IsSpecialAnimation(animationInfo, nullptr)))
        {
            GfxHAnim_SetRootMotionTime(
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                animationInfo, startTime);
            GfxHAnim_ApplyRootMotion(
                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                reinterpret_cast<uint32_t*>(animationInfo),
                GfxHAnim_GetBlendDestination(track));
        }
        C_00861e98_UpdateHierarchyPose(
            self, nullptr, GfxHAnim_GetBlendDestination(track));
    }
    else if ((track[5] & 2) != 0)
    {
        C_00861e98_PrepareBlend(self, nullptr, track);
        CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x10) = track[5];
        CSF_AT(int, reinterpret_cast<void*>(track[1]), 0x14) = track[7];

        int hierarchy = track[0];
        CSF_AT(float, track, 0x08) = blendDuration;
        int flags = track[5] | 8;
        track[3] = 0;
        track[5] = flags;

        uint32_t* interpolator = GfxHAnim_GetHierarchyInterpolator(hierarchy);
        if (!interpolator[0])
            RtAnimInterpolatorSetCurrentAnim(interpolator, animation);

        interpolator =
            GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendSource(track));
        if (!interpolator[0])
            RtAnimInterpolatorSetCurrentAnim(interpolator, animation);

        GfxHAnim_BlendHierarchyPose(
            GfxHAnim_GetBlendSource(track), CSF_AT(int, self, 0x14), track[6]);

        int sourceInterpolator = reinterpret_cast<int>(
            GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendSource(track)));
        RtAnimInterpolatorCopy(
            GfxHAnim_GetHierarchyInterpolator(track[0]), sourceInterpolator);

        if ((CSF_AT(uint32_t, CSF_AT(void*, self, 0x04), 0x14) & 0x100) != 0 &&
            (CSF_AT(uint32_t, self, 0x30) & 0x80000000u) == 0)
        {
            C_00861e98_PrepareBlendRootMotion(
                self, nullptr, reinterpret_cast<uint32_t*>(track),
                reinterpret_cast<int>(animation));
        }

        RtAnimInterpolatorSetCurrentAnim(
            GfxHAnim_GetHierarchyInterpolator(GfxHAnim_GetBlendDestination(track)),
            animation);
        RtAnimInterpolatorSetCurrentTime(
            reinterpret_cast<int>(GfxHAnim_GetHierarchyInterpolator(
                GfxHAnim_GetBlendDestination(track))),
            startTime);
        C_00861e98_UpdateHierarchyPose(
            self, nullptr, GfxHAnim_GetBlendDestination(track));
    }

    track[4] = track[2];
    uint32_t flags = static_cast<uint32_t>(track[5]);
    if (CSF_AT(uint8_t, options, 0x0C))
        flags |= 1;
    else
        flags &= 0xFFFFFFFEu;
    track[5] = flags;

    bool wasActive = ((flags >> 2) & 1) != 0;
    flags = (flags & 0xFFFFFFDBu) | 4;
    track[5] = flags;
    if ((flags & 2) != 0 && !wasActive)
        C_00861e98_PreparePrimaryTrack(self, nullptr);

    if (!trackIndex)
    {
        flags = static_cast<uint32_t>(track[5]);
        if (CSF_AT(uint8_t, options, 0x1C))
            flags |= 0x100;
        else
            flags &= 0xFFFFFEFFu;
        track[5] = flags;
        track[5] = flags & 0xFFFFFF3Fu;
        flags &= 0xFFFFFF3Fu;

        if (CSF_AT(uint8_t, options, 0x0D))
            track[5] = flags | 0x40;
        if (CSF_AT(uint8_t, options, 0x0E))
            track[5] |= 0x80u;

        if (CSF_AT(uint8_t, options, 0x0D) ||
            CSF_AT(uint8_t, options, 0x0E))
        {
            if ((CSF_AT(uint32_t, self, 0x58) & 1) != 0)
            {
                if ((CSF_AT(uint32_t, self, 0x30) & 0x80000000u) == 0 &&
                    (CSF_AT(uint32_t, self, 0x34) & 0x80000000u) == 0)
                {
                    uint32_t interpolatorAddress =
                        CSF_AT(uint32_t, CSF_AT(void*, self, 0x14), 0x20);
                    C_00861e98_CalculateRootMotion(
                        self, nullptr,
                        static_cast<int>(
                            CSF_AT(uint32_t, self, 0x34) *
                                CSF_AT(uint32_t, reinterpret_cast<void*>(interpolatorAddress), 0x24) +
                            interpolatorAddress + 76),
                        static_cast<int>(
                            CSF_AT(uint32_t, self, 0x30) *
                                CSF_AT(uint32_t, reinterpret_cast<void*>(interpolatorAddress), 0x24) +
                            interpolatorAddress + 76),
                        static_cast<int>(interpolatorAddress + 76),
                        reinterpret_cast<uint32_t*>(
                            reinterpret_cast<uint8_t*>(self) + 0x24),
                        nullptr, nullptr, nullptr);
                    CSF_AT(float, self, 0x24) = g_ZeroVectorX;
                    CSF_AT(float, self, 0x28) = g_ZeroVectorY;
                    CSF_AT(float, self, 0x2C) = g_ZeroVectorZ;
                }
            }
            else
            {
                if ((CSF_AT(uint32_t, self, 0x34) & 0x80000000u) != 0)
                    CSF_AT(uint32_t, self, 0x34) = 0;

                uint32_t* rotation = reinterpret_cast<uint32_t*>(
                    CSF_AT(uint32_t, self, 0x34) *
                        CSF_AT(uint32_t,
                            CSF_AT(void*, CSF_AT(void*, self, 0x14), 0x20), 0x24) +
                    CSF_AT(uint32_t, CSF_AT(void*, self, 0x14), 0x20) + 84);
                CSF_AT(uint32_t, self, 0x38) = rotation[0];
                CSF_AT(uint32_t, self, 0x3C) = rotation[1];
                CSF_AT(uint32_t, self, 0x40) = rotation[2];
                CSF_AT(uint32_t, self, 0x44) = rotation[3];
            }

            std::memcpy(
                CSF_AT(void*, self, 0x48),
                reinterpret_cast<uint8_t*>(
                    CSF_AT(void*, CSF_AT(void*, self, 0x1C), 0x04)) + 16,
                0x40u);
        }
    }

    C_00861e98_UpdateAnimationState(self, nullptr);
    track[7] = reinterpret_cast<int>(animationInfo);
    track[8] = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;

    if (!trackIndex)
    {
        uint32_t controllerFlags = CSF_AT(uint32_t, self, 0x58) | 0x10;
        CSF_AT(uint32_t, self, 0x58) = controllerFlags;
        if (CSF_AT(uint8_t, options, 0x1D))
        {
            CSF_AT(uint32_t, self, 0x58) = controllerFlags | 8;
            return 1;
        }
        CSF_AT(uint32_t, self, 0x58) = controllerFlags & 0xFFFFFFF7u;
    }
    return 1;
}

HOOK(0x006EA4D0, C_00861e98_StartAnimation, "auto");
