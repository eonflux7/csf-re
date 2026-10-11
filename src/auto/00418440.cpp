#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Resets the wheel-state fields of C_0082e1e8 to their defaults and returns 1.
static int __fastcall C_0082e1e8_ResetWheelState(C_0082e1e8* self, void* /*edx*/) {
    SceneLoader_NoOp();
    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->field_006c = 0.0f;
    self->field_0070 = 0.0f;
    self->field_0074 = 0.0f;
    self->field_0078 = 0.0f;
    self->field_0088 = 0;
    self->field_007c = 0;
    self->field_0085 = 1;
    self->field_0086 = 1;
    self->field_007c = 1;
    return 1;
}

HOOK(0x418440, C_0082e1e8_ResetWheelState, "auto");
