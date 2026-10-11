#include <cstdint>
#include "hook.h"

// Updates a cover-state record: when its enable byte is set, adds a2 to the float at offset 4.
static void __fastcall AiCoverAttackTasks_UpdateCoverState(float* self, void* /*edx*/, float a2) {
    if (*reinterpret_cast<uint8_t*>(self) != 0) {
        self[1] = a2 + self[1];
    }
}

HOOK(0x4D1120, AiCoverAttackTasks_UpdateCoverState, "auto");
