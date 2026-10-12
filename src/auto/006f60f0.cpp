#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006F60A0, __thiscall-style callee taking the scene controller as `this`.
CSF_FUNC(void, __fastcall, sub_6F60A0, 0x006F60A0, (void* self, void* edx));

// Sets the controller's ready flag at +0x04 after running its reset step.
// Returns TRUE (1) as a byte result.
static char __fastcall C_00862874_InitializeSceneController(C_00862874* self, void* /*edx*/)
{
    sub_6F60A0(self, nullptr);
    CSF_AT(uint8_t, self, 0x04) = 1;
    return 1;
}

HOOK(0x006F60F0, C_00862874_InitializeSceneController, "auto");
