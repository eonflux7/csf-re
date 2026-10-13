#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(float*, __stdcall, GfxAnimCompress_InterpolateChannelPose,
         0x006C2E40, (int a1));
CSF_FUNC(float*, __fastcall, HAnim_ApplyChannelPose, 0x006C3320,
         (void* self, void* edx, int a2, float* a3, float a4));

// The packet also identifies this address as a vtable reference.
static const auto kChannelPoseVtable = CSF_VTBL(0x006C3320);

static void __fastcall S_006c3320_AdvanceAnimationChannel(
    S_006c3320* self, void* /*edx*/, int channelAddress,
    float elapsedTime, float loopFlag)
{
    // The original comparison also lets an unordered value through.
    if (elapsedTime <= g_fZero)
        return;

    auto* channel = reinterpret_cast<std::uint8_t*>(channelAddress);
    const std::uint32_t channelIndex =
        CSF_AT(std::uint32_t, channel, 0x18);
    auto* keyframes = CSF_AT(
        std::uint8_t*, self, 4 + channelIndex * 4);

    if (CSF_AT(std::uint32_t, keyframes, 0) == 0)
        return;

    const float currentTime =
        elapsedTime + CSF_AT(float, channel, 0x08);
    CSF_AT(float, channel, 0x08) = currentTime;

    auto* duration = reinterpret_cast<std::uint8_t*>(self->field_0000) + 0x0C;
    if (currentTime > CSF_AT(float, duration, 0))
    {
        if (reinterpret_cast<const std::uint8_t*>(&loopFlag)[0] != 0)
        {
            float wrappedTime;
            do
            {
                wrappedTime = CSF_AT(float, channel, 0x08)
                            - CSF_AT(float, duration, 0);
                CSF_AT(float, channel, 0x08) = wrappedTime;
                duration =
                    reinterpret_cast<std::uint8_t*>(self->field_0000) + 0x0C;
            }
            while (wrappedTime > CSF_AT(float, duration, 0));

            HAnim_ApplyChannelPose(
                self, nullptr, channelAddress,
                reinterpret_cast<float*>(channelIndex), wrappedTime);
        }
        else
        {
            const std::uint32_t lastKeyframe =
                16u * CSF_AT(std::uint32_t, keyframes, 0x04)
                + CSF_AT(std::uint32_t, keyframes, 0) - 16u;
            CSF_AT(std::uint32_t, channel, 0) = lastKeyframe;
            CSF_AT(std::uint32_t, channel, 0x04) = lastKeyframe;

            const std::uint32_t durationBits = CSF_AT(
                std::uint32_t,
                reinterpret_cast<void*>(self->field_0000), 0x0C);
            auto* finalPose = CSF_AT(std::uint8_t*, channel, 0);
            CSF_AT(std::uint32_t, channel, 0x08) = durationBits;
            CSF_AT(std::uint32_t, channel, 0x0C) =
                CSF_AT(std::uint32_t, finalPose, 0x04);
            CSF_AT(std::uint32_t, channel, 0x10) =
                CSF_AT(std::uint32_t, finalPose, 0x08);
            CSF_AT(std::uint32_t, channel, 0x14) =
                CSF_AT(std::uint32_t, finalPose, 0x0C);
        }
        return;
    }

    auto* nextKeyframe = CSF_AT(std::uint8_t*, channel, 0x04);
    if (currentTime > CSF_AT(float, nextKeyframe, 0))
    {
        std::int32_t keyframeIndex = static_cast<std::int32_t>(
            (reinterpret_cast<std::uint32_t>(nextKeyframe)
             - CSF_AT(std::uint32_t, keyframes, 0)) >> 4);

        if (keyframeIndex < CSF_AT(std::int32_t, keyframes, 0x04))
        {
            std::uint32_t keyframeOffset =
                16u * static_cast<std::uint32_t>(keyframeIndex);
            for (;;)
            {
                const std::uint32_t keyframeAddress =
                    CSF_AT(std::uint32_t, keyframes, 0) + keyframeOffset;
                auto* keyframe = reinterpret_cast<void*>(keyframeAddress);

                if (!(CSF_AT(float, keyframe, 0)
                      <= CSF_AT(float, channel, 0x08)))
                {
                    CSF_AT(std::uint32_t, channel, 0x04) =
                        CSF_AT(std::uint32_t, keyframes, 0)
                        + 16u * static_cast<std::uint32_t>(keyframeIndex);
                    break;
                }

                CSF_AT(std::uint32_t, channel, 0) = keyframeAddress;
                CSF_AT(std::uint32_t, channel, 0x04) = keyframeAddress;
                ++keyframeIndex;
                keyframeOffset += 16;
                if (keyframeIndex >= CSF_AT(std::int32_t, keyframes, 0x04))
                    break;
            }
        }
    }

    GfxAnimCompress_InterpolateChannelPose(channelAddress);
}

HOOK(0x006C33C0, S_006c3320_AdvanceAnimationChannel, "auto");
