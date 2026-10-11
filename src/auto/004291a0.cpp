#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00428E00, __fastcall-style callee taking (this, a2), pops 4 bytes.
CSF_FUNC(char, __fastcall, CheatEntry_Apply, 0x00428E00, (void* self, void* edx, void* a2));

// Returns the result of CheatEntry_Apply when the enabled flag at +0xE98 is set, otherwise 0.
// The original tail-jumps to 0x428E00 with the same stack frame, so this forwards the
// argument and returns the callee's result; the callee pops the argument, matching ret 4.
static char __fastcall C_008300c0_ForwardCheatIfEnabled(C_008300c0* self, void* /*edx*/, void* a2)
{
    if (self->field_0e98) {
        return CheatEntry_Apply(self, nullptr, a2);
    }
    return 0;
}

HOOK(0x004291A0, C_008300c0_ForwardCheatIfEnabled, "auto");
