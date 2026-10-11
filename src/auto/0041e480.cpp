#include <cstdint>
#include "globals.h"
#include "hook.h"

// Shifts the stored pair of values inside a task-state object: copies the
// float at +0x0C into +0x08 and +0x04, and copies the dword at +0x14 into +0x10.
// Returns the float at +0x0C (widened to double, as the original's x87 return).
static double __fastcall TaskStates_ShiftSnapshot(void* self, void* /*edx*/)
{
    double current = CSF_AT(float, self, 0x0C);
    uint32_t savedDword = CSF_AT(uint32_t, self, 0x14);

    CSF_AT(float, self, 0x08) = CSF_AT(float, self, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = savedDword;
    CSF_AT(float, self, 0x04) = (float)current;

    return current;
}

HOOK_KEEP_ECX(0x0041E480, TaskStates_ShiftSnapshot, 0, "ai_task_states");

