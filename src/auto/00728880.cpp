#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static void __fastcall C_00865980_SetDurationSource(
    C_00865980* self, void* /*edx*/, void* source)
{
    void* previousSource = reinterpret_cast<void*>(self->field_0030);
    if (source == previousSource)
        return;

    if (previousSource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, previousSource, 0)[0])(
                previousSource, nullptr, 1);
        self->field_0030 = 0;
    }

    self->field_0030 = reinterpret_cast<std::uint32_t>(source);
    if (!source)
        return;

    void* sourceData =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[12])(source, nullptr);

    // Preserve all four original reads, including the unused components.
    (void)CSF_AT(volatile std::uint32_t, sourceData, 0x04);
    (void)CSF_AT(volatile std::uint32_t, sourceData, 0x00);
    const std::uint32_t durationBits =
        CSF_AT(volatile std::uint32_t, sourceData, 0x0C);
    (void)CSF_AT(volatile std::uint32_t, sourceData, 0x08);

    float duration;
    std::memcpy(&duration, &durationBits, sizeof(duration));

    self->field_0040 = duration * g_HalfDurationScale;
    std::memcpy(&self->field_0044, &durationBits, sizeof(durationBits));
    std::memcpy(&self->field_003c, &durationBits, sizeof(durationBits));
}

HOOK(0x00728880, C_00865980_SetDurationSource, "auto");
