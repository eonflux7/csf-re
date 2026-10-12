#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Resets the per-object state block of C_0082e090 to its default values.
// The original zeroes most fields and sets the byte at +0x78 to 1.
static int __fastcall C_0082e090_ResetSceneState(C_0082e090* self, void* /*edx*/) {
    SceneLoader_NoOp();

    self->field_0084 = 0.0f;
    CSF_AT(float, self, 0x88) = 0.0f;
    CSF_AT(float, self, 0x8C) = 0.0f;
    self->field_009c = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    self->field_0075 = 0;
    self->field_0076 = 0;
    self->field_0077 = 0;
    self->field_0078 = 1;
    self->field_007c = 0;
    self->field_0080 = 0.0f;
    self->field_0074 = 0;
    self->field_0090 = 0;
    self->field_00b8 = 0.0f;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    return 0;
}

HOOK(0x415DA0, C_0082e090_ResetSceneState, "auto");
