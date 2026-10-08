#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CMenuPause::ResetState (0x00539CE0, 40 bytes)
// Clears the per-frame/button state of the pause menu: resets most button
// pointers/state fields to 0, resets field_0014 to -1, and always returns 0.
static int __fastcall CMenuPause_ResetState(CMenuPause* self, void* /*edx*/)
{
    self->field_0010 = 0;
    self->field_003c = 0;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_001c = 0;
    self->field_0034 = 0;
    self->field_0024 = 0;
    self->field_0014 = 0xFFFFFFFFu;
    self->field_0040 = 0;
    return 0;
}

HOOK(0x00539CE0, CMenuPause_ResetState, "auto");
