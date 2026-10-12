#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resets the per-task state block of a movement task (TMovilAPunto_2 family).
// Layout is not declared in types/auto_classes.h, so offsets are accessed directly.
static int __fastcall AiMoveTasks_ResetTaskState(void* self, void* /*edx*/, int /*a2*/)
{
    CSF_AT(int, self, 0x04) = 1;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(uint8_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x15) = 1;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(uint8_t, self, 0x24) = 1;
    return 0;
}

HOOK(0x004ABB60, AiMoveTasks_ResetTaskState, "auto");
