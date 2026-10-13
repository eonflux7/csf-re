#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint32_t GfxAnimCompress_DecodeComponentBits(std::uint16_t packed)
{
    std::uint32_t bits = (packed & 0x8000u) << 16;
    if ((packed & 0x7FFF) != 0)
    {
        bits |= ((packed & 0x7FF) << 12)
              | (((packed & 0x7800) + 229376) << 12);
    }
    return bits;
}

static int __fastcall S_006c20f0_DecompressKeyframeChain(
    S_006c20f0* self, void* /*edx*/, std::uint32_t* output, int firstFrame)
{
    output[2] = firstFrame;

    void* animation = reinterpret_cast<void*>(self->field_0000);
    auto* frames = CSF_AT(std::uint8_t*, animation, 0x10);
    auto* translationParameters = CSF_AT(float*, animation, 0x14);
    const int frameCount = CSF_AT(int, animation, 0x04);

    std::uint8_t* previousFrame = nullptr;
    int selectedCount = 0;

    for (int index = 0; index < frameCount; ++index)
    {
        auto* frame = frames + 24 * index;
        if (index == firstFrame)
        {
            previousFrame = frame;
            if (CSF_AT(float, frame, 0x04) != g_fZero)
                break;
        }
        else
        {
            if (index <= firstFrame
                || CSF_AT(std::uint8_t*, frame, 0x00) != previousFrame
                || selectedCount <= 0)
                continue;
            previousFrame = frame;
        }
        ++selectedCount;
    }

    if (selectedCount > 0)
    {
        output[1] = selectedCount;
        output[0] = CoreMemory_AllocUntagged(
            static_cast<std::uint32_t>(selectedCount) * 16);

        selectedCount = 0;
        previousFrame = nullptr;
        int index = 0;
        int outputOffset = 0;

        if (CSF_AT(int, reinterpret_cast<void*>(self->field_0000), 0x04) > 0)
        {
            auto* frame = frames;
            do
            {
                if (index == firstFrame
                    || (index > firstFrame
                        && CSF_AT(std::uint8_t*, frame, 0x00) == previousFrame))
                {
                    previousFrame = frame;
                    const std::uint32_t time = CSF_AT(std::uint32_t, frame, 0x04);
                    CSF_AT(std::uint32_t,
                           reinterpret_cast<void*>(output[0]), outputOffset) = time;

                    auto* destination =
                        reinterpret_cast<std::uint8_t*>(output[0]) + outputOffset;

                    CSF_AT(std::uint32_t, destination, 0x04) =
                        GfxAnimCompress_DecodeComponentBits(
                            CSF_AT(std::uint16_t, frame, 0x10));
                    CSF_AT(std::uint32_t, destination, 0x08) =
                        GfxAnimCompress_DecodeComponentBits(
                            CSF_AT(std::uint16_t, frame, 0x12));
                    CSF_AT(std::uint32_t, destination, 0x0C) =
                        GfxAnimCompress_DecodeComponentBits(
                            CSF_AT(std::uint16_t, frame, 0x14));

                    ++selectedCount;
                    outputOffset += 16;

                    CSF_AT(float, destination, 0x04) =
                        translationParameters[3] * CSF_AT(float, destination, 0x04)
                        + translationParameters[0];
                    CSF_AT(float, destination, 0x08) =
                        translationParameters[4] * CSF_AT(float, destination, 0x08)
                        + translationParameters[1];
                    CSF_AT(float, destination, 0x0C) =
                        translationParameters[5] * CSF_AT(float, destination, 0x0C)
                        + translationParameters[2];
                }

                ++index;
                frame += 24;
            }
            while (index < CSF_AT(
                int, reinterpret_cast<void*>(self->field_0000), 0x04));
        }
    }

    return selectedCount > 0;
}

HOOK(0x006C20F0, S_006c20f0_DecompressKeyframeChain, "auto");
