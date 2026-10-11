#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall with two stack args (ret 8): ecx = this, edx unused.
// Sets the cover-transition flag byte at +0 to 1, stores a2 at +4 and a3 at +8, returns a2.
static int __fastcall AiCoverAttackTasks_SetCoverTransition(void* self, void* /*edx*/, int a2, int a3)
{
    CSF_AT(uint8_t, self, 0x00) = 1;
    CSF_AT(int, self, 0x04) = a2;
    CSF_AT(int, self, 0x08) = a3;
    return a2;
}

HOOK(0x4D1100, AiCoverAttackTasks_SetCoverTransition, "auto");
