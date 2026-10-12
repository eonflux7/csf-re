#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Resets the scene mission setup object's option block to defaults and returns it.
// Fields are written in the same order as the original; offsets are not declared
// in types/auto_classes.h, so they are accessed through CSF_AT.
static uint32_t* __fastcall SceneMissionSetup_InitializeObjectOptions(void* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(float, self, 0x54) = 1.0f;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(float, self, 0x5C) = 0.5f;
    return static_cast<uint32_t*>(self);
}

HOOK(0x6B2750, SceneMissionSetup_InitializeObjectOptions, "auto");
