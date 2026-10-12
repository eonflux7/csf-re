#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// Advances a fade timer by elapsed. While the timer is below the fade duration (field_0008),
// it publishes the remaining fade amount (0..255) to +0x24 and forwards to the target object's
// slot 24 with a pointer to +0x18. Otherwise it resets the fade state and returns 0.
static int __fastcall S_004382f0_AdvanceFadeAndNotify(S_004382f0* self, void* /*edx*/, float elapsed) {
    float fadeTimer = elapsed + self->field_0010;
    self->field_0010 = fadeTimer;

    if (fadeTimer < self->field_0008) {
        CSF_AT(float, self, 0x24) =
            g_ColorChannelScale - fadeTimer / self->field_0008 * g_ColorChannelScale;

        void* target = reinterpret_cast<void*>(self->field_0004);
        void* arg = reinterpret_cast<uint8_t*>(self) + 0x18;
        return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[96 / 4])(
            target, nullptr, arg);
    }

    self->field_0000 = 0xFFFFFFFFu;
    self->field_0010 = 0.0f;
    self->field_0014 = 0;
    return 0;
}

HOOK(0x00438350, S_004382f0_AdvanceFadeAndNotify, "auto");
