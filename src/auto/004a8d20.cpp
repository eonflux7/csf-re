#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a 16-byte recoil record: two 32-bit values from the arguments,
// two zeroed flag bytes, and a float 1.0f at +0x0C. Returns the record pointer.
static void* __fastcall InitializeRecoilData(void* self, void* /*edx*/, int32_t a2, int32_t a3)
{
    CSF_AT(int32_t, self, 0x00) = a2;
    CSF_AT(int32_t, self, 0x04) = a3;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(uint8_t, self, 0x09) = 0;
    CSF_AT(float, self, 0x0C) = 1.0f;
    return self;
}

HOOK(0x4A8D20, InitializeRecoilData, "auto");
