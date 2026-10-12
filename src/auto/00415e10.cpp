#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);

// One-shot decay: on the first call (field_0078 == 0), marks the object as stepped,
// scales down the two float state values by (1 - 0.75) and clamps a positive
// field_0088 to zero. Later calls do nothing.
static void __fastcall C_0082e090_DampenOnceIfIdle(C_0082e090* self, void* /*edx*/) {
    if (self->field_0078 != 0) {
        return;
    }

    float previous084 = self->field_0084;
    self->field_0078 = 1;
    self->field_0084 = self->field_0084 - previous084 * g_CharacterWidthScale;
    self->field_008c = self->field_008c - self->field_008c * g_CharacterWidthScale;

    if (self->field_0088 > g_fZero) {
        self->field_0088 = 0.0f;
    }
}

HOOK(0x00415E10, C_0082e090_DampenOnceIfIdle, "auto");
