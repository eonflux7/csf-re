#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0082faf8_UpdateTransition(
    C_0082faf8* self, void* /*edx*/, float deltaTime)
{
    const float value = deltaTime * self->field_0058 + self->field_0050;
    self->field_0050 = value;

    if (self->field_0058 > g_fZero && value >= self->field_005c)
    {
        self->field_0050 = self->field_005c;
        self->field_0058 = 0.0f;
    }
    else if (self->field_0058 < g_fZero && value <= self->field_005c)
    {
        self->field_0050 = self->field_005c;
        self->field_0058 = 0.0f;
    }

    void* state = reinterpret_cast<void*>(self->field_0028);
    if (self->field_0074 != CSF_AT(uint8_t, state, 0x20))
    {
        self->field_0050 = 0.1f;
        self->field_0074 = CSF_AT(uint8_t, state, 0x20);

        void* settings = reinterpret_cast<void*>(self->field_0014);
        self->field_0058 = CSF_AT(float, settings, 0x194);

        if (self->field_0074)
        {
            const float limit = CSF_AT(float, settings, 0x188);
            self->field_005c =
                self->field_0054 < limit ? self->field_0054 : limit;
        }
        else
        {
            self->field_005c = self->field_0054;
        }
    }
}

HOOK_KEEP_ECX(0x00424130, C_0082faf8_UpdateTransition, 1, "auto");
