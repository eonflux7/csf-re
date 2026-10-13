#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static void __fastcall S_0073b580_AdvanceFade(
    S_0073b580* self, void* /*edx*/, float delta)
{
    const double halfDuration =
        static_cast<double>(self->field_0014) * g_HalfDurationScale;

    self->field_0010 = static_cast<float>(
        static_cast<double>(self->field_001c) * g_ColorChannelScale /
        halfDuration);

    const double progress = static_cast<double>(delta) + self->field_001c;
    self->field_001c = static_cast<float>(progress);

    if (progress >= halfDuration) {
        self->field_0010 = 255.0f;
        self->field_0024 = 1;
    }
}

HOOK(0x0073B580, S_0073b580_AdvanceFade, "auto");
