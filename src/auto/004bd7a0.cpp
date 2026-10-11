#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the AI state block of an object to its default values.
// The first argument is stored in the first two dwords (identity/owner slots).
static int __fastcall AiCore_InitStateDefaults(void* self, void* /*edx*/, int32_t ownerId) {
    CSF_AT(int32_t, self, 0x00) = ownerId;
    CSF_AT(int32_t, self, 0x04) = ownerId;

    CSF_AT(int32_t, self, 0x28) = -1;
    CSF_AT(int32_t, self, 0x2c) = -1;
    CSF_AT(int32_t, self, 0x30) = -1;
    CSF_AT(int32_t, self, 0x38) = -1;
    CSF_AT(int32_t, self, 0x3c) = -1;
    CSF_AT(int32_t, self, 0x50) = -1;
    CSF_AT(int32_t, self, 0x54) = -1;

    CSF_AT(int32_t, self, 0x5c) = 0;
    CSF_AT(float, self, 0x60) = 1.0f;
    CSF_AT(float, self, 0x9c) = FLT_MAX;
    CSF_AT(int32_t, self, 0x94) = 0;

    CSF_AT(uint8_t, self, 0xa8) = 0;
    CSF_AT(uint8_t, self, 0xa9) = 0;
    CSF_AT(int32_t, self, 0xa0) = 0;
    CSF_AT(int32_t, self, 0xa4) = 0;

    return 0;
}

HOOK(0x4BD7A0, AiCore_InitStateDefaults, "auto");
