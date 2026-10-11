#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies the 12-byte value block at this+0x4C into the caller's buffer and returns it.
static uint32_t* __fastcall C_00863430_GetTargetTransform(C_00863430* self, void* /*edx*/, uint32_t* outBuffer) {
    outBuffer[0] = CSF_AT(uint32_t, self, 0x4C);
    outBuffer[1] = CSF_AT(uint32_t, self, 0x50);
    outBuffer[2] = CSF_AT(uint32_t, self, 0x54);
    return outBuffer;
}

HOOK(0x00409470, C_00863430_GetTargetTransform, "auto");
