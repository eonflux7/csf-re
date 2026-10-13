#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x82D444);

// Advances the accumulated time by deltaTime. Once the accumulated time reaches
// field_0018 + 0.5 * field_0014, sets state field_0024 to 2 and field_0010 to 255.0.
static void __fastcall S_0073b580_AdvanceTimer(S_0073b580* self, void* /*edx*/, float deltaTime) {
    float elapsed = deltaTime + self->field_001c;
    self->field_001c = elapsed;
    if (elapsed >= self->field_0014 * g_HalfDurationScale + self->field_0018) {
        self->field_0024 = 2;
        self->field_0010 = 255.0f;
    }
}

HOOK(0x0073B5C0, S_0073b580_AdvanceTimer, "auto");
