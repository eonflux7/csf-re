#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtAnimInterpolatorCopy, 0x0061F120, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorAddAnimTime, 0x0061F150, (int a1, float a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));
CSF_FUNC(void, __fastcall, GfxHAnim_AdvanceAnimationChannel, 0x006C33C0, (void* self, void* edx, int a2, float a3, float a4));
CSF_FUNC(uint32_t*, __stdcall, GfxHAnim_ApplyAnimationChannels, 0x006E85B0, (int a1, uint32_t* a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_00861e98_PrepareAnimationTransition, 0x006E8D80, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, C_00861e98_SetTransitionHierarchy, 0x006E9BF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00861e98_FinishAnimationTransition, 0x006E9D60, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AnimationEndEpsilon, 0x00861E00);

static void __fastcall C_00861e98_AdvanceAnimation(
    C_00861e98* self, void* /*edx*/,
    int animationState, int hierarchy, uint8_t* wrapped, float deltaTime)
{
    auto* state = reinterpret_cast<uint8_t*>(animationState);
    auto* hierarchyObject = reinterpret_cast<uint8_t*>(hierarchy);
    auto* interpolator = CSF_AT(uint8_t*, hierarchyObject, 0x20);
    auto* animation = CSF_AT(uint8_t*, interpolator, 0);
    const float previousTime = CSF_AT(float, interpolator, 4);
    const float duration = CSF_AT(float, animation, 0xC);

    if (wrapped)
        *wrapped = 0;

    RtAnimInterpolatorAddAnimTime(
        CSF_AT(int, hierarchyObject, 0x20), deltaTime);

    if (previousTime + deltaTime > duration)
    {
        if (CSF_AT(uint8_t, state, 0x14) & 1)
        {
            if (wrapped)
                *wrapped = 1;
        }
        else
        {
            RtAnimInterpolatorSetCurrentTime(
                CSF_AT(int, hierarchyObject, 0x20),
                duration - g_AnimationEndEpsilon);

            uint32_t flags = CSF_AT(uint32_t, state, 0x14) | 0x20;
            CSF_AT(uint32_t, state, 0x14) = flags;
            if (flags & 2)
            {
                const float transitionDuration = CSF_AT(float, state, 0x10);
                flags &= 0xFFFFFFF7;
                CSF_AT(uint32_t, state, 0x14) = flags;

                if (!(transitionDuration > g_fZero))
                {
                    CSF_AT(uint32_t, state, 0x14) = flags & 0xFFFFFFFB;
                    C_00861e98_FinishAnimationTransition(self, nullptr);
                }
                else
                {
                    C_00861e98_PrepareAnimationTransition(
                        self, nullptr, reinterpret_cast<int*>(state));

                    auto* sourceHierarchy = CSF_AT(uint8_t*, state, 0);
                    const int sourceInterpolator =
                        CSF_AT(int, sourceHierarchy, 0x20);
                    auto* transition = CSF_AT(uint8_t*, state, 4);
                    auto* destinationHierarchy =
                        CSF_AT(uint8_t*, transition, 8);
                    RtAnimInterpolatorCopy(
                        CSF_AT(uint32_t*, destinationHierarchy, 0x20),
                        sourceInterpolator);

                    transition = CSF_AT(uint8_t*, state, 4);
                    C_00861e98_SetTransitionHierarchy(
                        self, nullptr, CSF_AT(int, transition, 8));

                    const uint32_t channelData = CSF_AT(uint32_t, state, 0x1C);
                    transition = CSF_AT(uint8_t*, state, 4);
                    CSF_AT(uint32_t, transition, 0x14) = channelData;

                    flags = CSF_AT(uint32_t, state, 0x14) | 0x10;
                    CSF_AT(uint32_t, state, 8) = CSF_AT(uint32_t, state, 0x10);
                    CSF_AT(uint32_t, state, 0xC) = 0;
                    CSF_AT(uint32_t, state, 0x14) = flags;
                }
            }
        }
    }

    if (!(CSF_AT(uint8_t, state, 0x14) & 2) &&
        CSF_AT(uint32_t, state, 0x1C))
    {
        auto* channels = reinterpret_cast<uint8_t*>(self) + 0x60;
        for (int index = 0; index < 8; ++index)
        {
            const int channel = CSF_AT(int, channels, index * 4);
            if (channel)
            {
                const uint32_t loopBits = CSF_AT(uint8_t, state, 0x14) & 1;
                float loopArgument;
                std::memcpy(&loopArgument, &loopBits, sizeof(loopArgument));
                GfxHAnim_AdvanceAnimationChannel(
                    CSF_AT(void*, state, 0x1C), nullptr,
                    channel, deltaTime, loopArgument);
            }
        }

        GfxHAnim_ApplyAnimationChannels(
            reinterpret_cast<int>(channels),
            CSF_AT(uint32_t*, state, 0x1C), hierarchy);
    }
}

HOOK(0x006EADC0, C_00861e98_AdvanceAnimation, "auto");
