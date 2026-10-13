#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_TransitionEpsilon, 0x00867480);

static void __fastcall C_0086754c_UpdateTransition(
    C_0086754c* self, void* /*edx*/, float deltaTime)
{
    if (!self->field_0004 ||
        std::fabs(-self->field_0014) <= g_TransitionEpsilon)
        return;

    const bool decreasing = self->field_0018 != 0;
    const float elapsed = deltaTime + self->field_0010;
    self->field_0010 = elapsed;

    float scale = static_cast<float>(
        static_cast<int32_t>(self->field_0020));
    if (self->field_0020 & 0x80000000)
        scale += g_UnsignedIntegerBias;

    const uint32_t progress = static_cast<uint32_t>(
        static_cast<int64_t>(scale * elapsed));

    if (decreasing)
    {
        const float change = deltaTime * self->field_002c;
        self->field_001c = self->field_0024 - progress;
        self->field_0028 = self->field_0028 - change;
    }
    else
    {
        const float change = deltaTime * self->field_002c;
        self->field_001c = progress;
        self->field_0028 = change + self->field_0028;
    }

    self->field_0028 = self->field_0028 < g_TransitionOne
        ? g_TransitionOne
        : self->field_0028;

    if (elapsed >= self->field_0014)
        self->field_0004 = 0;
}

HOOK(0x0073BB70, C_0086754c_UpdateTransition, "auto");
