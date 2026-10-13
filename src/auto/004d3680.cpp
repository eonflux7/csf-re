#include <cstdint>
#include "globals.h"
#include "hook.h"

// Ensures the scene entity's pending task flag (bit 0 of the task's flags at +4) is cleared
// when the entity owns a task with that flag set and the entity's +0x44 bit 2 is clear.
static int __fastcall FinalizeSceneEntityTask(void* self, void* /*edx*/) {
    uint32_t taskAddress = CSF_AT(uint32_t, self, 0x54);
    void* task = reinterpret_cast<void*>(taskAddress);

    if (taskAddress != 0 &&
        (CSF_AT(uint8_t, task, 0x4) & 1) != 0 &&
        (CSF_AT(uint8_t, self, 0x44) & 4) == 0) {
        CSF_AT(uint32_t, task, 0x4) &= ~1u;
    }
    return static_cast<int>(taskAddress);
}

HOOK(0x4D3680, FinalizeSceneEntityTask, "auto");
