#include <cstdint>
#include "hook.h"

// Sets a 4-dword quaternion to (0, 0, 0, 1.0f) and returns the same pointer.
static uint32_t* __fastcall GfxMatrix_InitializeQuaternion(uint32_t* self, void* /*edx*/) {
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0x3f800000; // float 1.0f, stored as its bit pattern as in the original
    return self;
}

HOOK(0x006AE6D0, GfxMatrix_InitializeQuaternion, "auto");
