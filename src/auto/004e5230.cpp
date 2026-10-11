#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"

// Sets the entity's action flag to 1 and stores the byte argument at +0x4D.
// Returns the byte argument (the original returns it in al).
static char __fastcall AiAutonomous_SetEntityActionFlag(void* self, void* /*edx*/, char flagValue) {
    CSF_AT(int, self, 0x30) = 1;
    CSF_AT(uint8_t, self, 0x4D) = static_cast<uint8_t>(flagValue);
    return flagValue;
}

HOOK(0x4E5230, AiAutonomous_SetEntityActionFlag, "auto");
