#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"

// Sets the autonomous AI's indexed action state: stores the target entity index,
// marks the action mode as 2, and records the flag byte. Returns the flag byte.
static uint8_t __fastcall AiAutonomous_SetEntityIndexedActionFlag(void* self, void* /*edx*/,
                                                                  int entityIndex, uint8_t flag) {
    CSF_AT(int, self, 0x30) = 2;
    CSF_AT(uint8_t, self, 0x4D) = flag;
    CSF_AT(int, self, 0x44) = entityIndex;
    return flag;
}

HOOK(0x4E5250, AiAutonomous_SetEntityIndexedActionFlag, "auto");
