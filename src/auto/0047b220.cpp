#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Clears the attack-task state flags on the task object (ecx = this).
// Sets the byte at +0x2A8 to 1 and the dword at +0x2B0 to 0.
static void __fastcall ResetAttackTaskState(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x2A8) = 1;
    CSF_AT(int, self, 0x2B0) = 0;
}

HOOK(0x47B220, ResetAttackTaskState, "auto");
