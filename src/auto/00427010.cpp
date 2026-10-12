#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_426DB0, 0x00426DB0, (void* a1));

// Clears the two counter fields of S_00426550, then tail-forwards `this` to sub_426DB0.
static void __fastcall S_00426550_ResetState(S_00426550* self, void* /*edx*/) {
    self->field_000c = 0;
    self->field_0024 = 0;
    sub_426DB0(self);
}

HOOK(0x00427010, S_00426550_ResetState, "auto");
