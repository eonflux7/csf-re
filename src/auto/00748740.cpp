#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the selected-character state of a multiplayer spawn-point object.
static int __fastcall C_00860288_ResetSelectedCharacter(C_00860288* self, void* /*edx*/) {
    // Byte-only write to the low byte of field_005c (original writes just one byte).
    *reinterpret_cast<uint8_t*>(&self->field_005c) = 1;

    CSF_AT(uint32_t, self, 0x2E8) = 0;
    CSF_AT(uint32_t, self, 0x2EC) = 0;
    CSF_AT(float, self, 0x32C) = 15.0f;
    CSF_AT(uint8_t, self, 0x300) = 0;
    CSF_AT(uint16_t, self, 0x2D6) = 0;
    CSF_AT(uint16_t, self, 0x2DC) = 0;
    CSF_AT(uint16_t, self, 0x2C4) = 0;

    return 0;
}

HOOK(0x00748740, C_00860288_ResetSelectedCharacter, "auto");
