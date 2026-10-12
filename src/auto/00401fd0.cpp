#include "hook.h"

// Adds a 3-component float vector in place: self += other. Returns self.
static float* __fastcall Vec3_AddAssign(float* self, void* /*edx*/, float* other) {
    self[0] = other[0] + self[0];
    self[1] = other[1] + self[1];
    self[2] = other[2] + self[2];
    return self;
}

HOOK(0x401FD0, Vec3_AddAssign, "auto");
