#include <cstdint>
#include "globals.h"
#include "hook.h"

// Adds a signed delta to the byte at +0x38 of the object and returns the delta.
// __thiscall(this, a2) is written as __fastcall(self, edx, a2); the single stack
// argument is popped by the callee (ret 4).
static char __fastcall AiAttackTasks_AddToActionFlag(uint8_t* self, void* /*edx*/, char delta)
{
    CSF_AT(uint8_t, self, 0x38) += static_cast<uint8_t>(delta);
    return delta;
}

HOOK(0x0048C9D0, AiAttackTasks_AddToActionFlag, "auto");
