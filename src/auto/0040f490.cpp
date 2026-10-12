#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00406720: S_004054A0 activation helper, __fastcall with this in ecx.
CSF_FUNC(char, __fastcall, S_004054A0_EnsureActivated, 0x00406720, (S_004054a0* self, void* edx));

// 0x0040F150: tail-jump target, takes this in ecx and returns char.
CSF_FUNC(char, __fastcall, sub_40F150, 0x0040F150, (void* self, void* edx));

// 0x0040F490: activates the linked object referenced by field_001c, then tail-jumps
// to sub_40F150 with the same this and returns its result.
static char __fastcall C_0082ddf8_GetLinkedObjectState(C_0082ddf8* self, void* /*edx*/)
{
    S_004054A0_EnsureActivated(reinterpret_cast<S_004054a0*>(self->field_001c), nullptr);
    return sub_40F150(self, nullptr);
}

HOOK(0x0040F490, C_0082ddf8_GetLinkedObjectState, "auto");
