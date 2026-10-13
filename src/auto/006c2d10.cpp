#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __cdecl, RtAnimAnimationCreate, 0x0061EE10, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, S_006c20f0_CompressTrack, 0x006C20F0, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(char, __fastcall, S_006c20f0_FinalizeAnimationFallback, 0x006C2410, (void* self, void* edx));

// The packet also classifies the directly called compression method as a vtable.
[[maybe_unused]] static const auto kCompressionMethodVtable = CSF_VTBL(0x006C20F0);

static char __fastcall S_006c20f0_CompressAnimation(
    S_006c20f0* self, void* /*edx*/, const int* trackIds)
{
    for (int trackIndex = 0; trackIndex < 8; ++trackIndex)
    {
        if (trackIds[trackIndex] == -1)
            break;

        uint32_t& trackState = CSF_AT(uint32_t, self, 0x4 + 4 * trackIndex);
        if (!trackState)
        {
            auto* state = reinterpret_cast<uint32_t*>(CoreMemory_AllocUntagged(12));
            if (state)
            {
                state[0] = 0;
                state[1] = 0;
                state[2] = static_cast<uint32_t>(-1);
            }
            trackState = reinterpret_cast<uint32_t>(state);
        }

        const int result = S_006c20f0_CompressTrack(
            self, nullptr, reinterpret_cast<uint32_t*>(trackState),
            trackIds[trackIndex]);
        if (static_cast<uint8_t>(result) == 0)
            return S_006c20f0_FinalizeAnimationFallback(self, nullptr);
    }

    auto* animation = reinterpret_cast<uint8_t*>(self->field_0000);
    const int flags = CSF_AT(int, animation, 0xC);
    const int duration = CSF_AT(int, animation, 0x8);
    const int frameCount = CSF_AT(int, animation, 0x4);
    char* compressedAnimation =
        RtAnimAnimationCreate(273, frameCount, duration, flags);

    animation = reinterpret_cast<uint8_t*>(self->field_0000);
    const int initialFrameCount = CSF_AT(int, animation, 0x4);
    const uint32_t sourceFramesAddress = CSF_AT(uint32_t, animation, 0x10);
    const uint32_t destinationFramesAddress =
        CSF_AT(uint32_t, compressedAnimation, 0x10);

    if (initialFrameCount > 0)
    {
        auto* sourceFrame = reinterpret_cast<uint8_t*>(sourceFramesAddress);
        auto* destinationFrame = reinterpret_cast<uint8_t*>(destinationFramesAddress);
        int frameIndex = 0;

        do
        {
            const uint32_t previousFrameOffset =
                CSF_AT(uint32_t, sourceFrame, 0x0) - sourceFramesAddress;
            CSF_AT(uint32_t, destinationFrame, 0x0) =
                destinationFramesAddress + 16 * (previousFrameOffset / 0x18u);
            CSF_AT(uint32_t, destinationFrame, 0x4) =
                CSF_AT(uint32_t, sourceFrame, 0x4);
            CSF_AT(uint16_t, destinationFrame, 0x8) =
                CSF_AT(uint16_t, sourceFrame, 0x8);
            CSF_AT(uint16_t, destinationFrame, 0xA) =
                CSF_AT(uint16_t, sourceFrame, 0xA);
            CSF_AT(uint16_t, destinationFrame, 0xC) =
                CSF_AT(uint16_t, sourceFrame, 0xC);
            CSF_AT(uint16_t, destinationFrame, 0xE) =
                CSF_AT(uint16_t, sourceFrame, 0xE);

            const int currentFrameCount =
                CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0000), 0x4);
            ++frameIndex;
            sourceFrame += 0x18;
            destinationFrame += 0x10;
            if (frameIndex >= currentFrameCount)
                break;
        } while (true);
    }

    const char result =
        static_cast<char>(RtAnimInterpolatorDestroy(static_cast<int>(self->field_0000)));
    self->field_0000 = reinterpret_cast<uint32_t>(compressedAnimation);
    return result;
}

HOOK(0x006C2D10, S_006c20f0_CompressAnimation, "auto");
