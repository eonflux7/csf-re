#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __stdcall, GfxAnimCompress_InterpolateChannelPose, 0x006C2E40, (int a1));

static float* __fastcall S_006c3320_ApplyChannelPose(
    S_006c3320* self, void* /*edx*/,
    int poseAddress, float* channel, float time)
{
    void* pose = reinterpret_cast<void*>(poseAddress);
    CSF_AT(uint8_t*, pose, 0x0) = nullptr;
    CSF_AT(uint8_t*, pose, 0x4) = nullptr;
    CSF_AT(float*, pose, 0x18) = channel;

    const uint32_t channelIndex = reinterpret_cast<uint32_t>(channel);
    void* track = CSF_AT(void*, self, 0x4 + channelIndex * 4);
    if (CSF_AT(uint8_t*, track, 0x0) == nullptr)
        return channel;

    CSF_AT(float, pose, 0x8) = time;
    uint8_t* firstKey = CSF_AT(uint8_t*, track, 0x0);
    CSF_AT(uint8_t*, pose, 0x0) = firstKey;
    CSF_AT(uint8_t*, pose, 0x4) = firstKey;

    int32_t keyIndex = 1;
    if (CSF_AT(int32_t, track, 0x4) > 1)
    {
        uint32_t keyOffset = 0x10;
        for (;;)
        {
            uint8_t* key = CSF_AT(uint8_t*, track, 0x0) + keyOffset;
            if (!(CSF_AT(float, key, 0x0) <= time))
            {
                CSF_AT(uint8_t*, pose, 0x4) =
                    CSF_AT(uint8_t*, track, 0x0) +
                    static_cast<uint32_t>(keyIndex) * 0x10;
                break;
            }

            CSF_AT(uint8_t*, pose, 0x0) = key;
            CSF_AT(uint8_t*, pose, 0x4) = key;
            const int32_t keyCount = CSF_AT(int32_t, track, 0x4);
            ++keyIndex;
            keyOffset += 0x10;
            if (keyIndex >= keyCount)
                break;
        }
    }

    return GfxAnimCompress_InterpolateChannelPose(poseAddress);
}

HOOK(0x006C3320, S_006c3320_ApplyChannelPose, "auto");
