#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x82D444);
CSF_GLOBAL(float, g_ColorChannelScale, 0x82FEA8);

// Original: 0x0073B600, void __thiscall (S_0073b580* this, float elapsed); ret 4.
// Advances the timer at +0x1C, maps it to a 0..255 level at +0x10 over the
// half-duration span, and once the timer passes the end of the span, clears
// the level, +0x24 and the +0x04 flag.
static void __fastcall S_0073b580_AdvanceFadeAndUpdateLevel(S_0073b580* self, void* /*edx*/, float elapsed)
{
    self->field_0010 = g_ColorChannelScale
                     - (self->field_001c - (self->field_0014 * g_HalfDurationScale + self->field_0018))
                       * g_ColorChannelScale / (self->field_0014 * g_HalfDurationScale);

    self->field_001c = elapsed + self->field_001c;

    if (self->field_001c >= self->field_0014 + self->field_0018)
    {
        self->field_0010 = 0.0f;
        self->field_0024 = 0;
        self->field_0004 = 0;
    }
}

HOOK(0x73B600, S_0073b580_AdvanceFadeAndUpdateLevel, "auto");
