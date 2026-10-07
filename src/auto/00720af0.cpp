#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall ReplaceDurationSource(
    void* self, void* /*edx*/, void* source)
{
    void* previousSource = CSF_AT(void*, self, 0x134);
    if (previousSource == source)
        return;

    if (previousSource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousSource, 0)[0])(
                previousSource, nullptr, 1);
        CSF_AT(void*, self, 0x134) = nullptr;
    }

    CSF_AT(void*, self, 0x134) = source;

    void* parameters =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[25])(source, nullptr);

    // Preserve the original's otherwise-unused component reads.
    (void)CSF_AT(volatile std::uint32_t, parameters, 0x04);
    (void)CSF_AT(volatile std::uint32_t, parameters, 0x00);
    const std::uint32_t durationBits =
        CSF_AT(volatile std::uint32_t, parameters, 0x0C);

    float duration;
    std::memcpy(&duration, &durationBits, sizeof(duration));
    const float halfDuration = static_cast<float>(
        static_cast<double>(duration) *
        static_cast<double>(g_HalfDurationScale));

    (void)CSF_AT(volatile std::uint32_t, parameters, 0x08);

    CSF_AT(float, self, 0x14C) = halfDuration;
    CSF_AT(std::uint32_t, self, 0x150) = durationBits;
    CSF_AT(std::uint32_t, self, 0x148) = durationBits;
}

HOOK(0x00720AF0, ReplaceDurationSource, "auto");
