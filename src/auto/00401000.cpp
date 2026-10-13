#include "hook.h"

// Scales a 3-component float vector by a scalar: out = scale * in.
// Writes the result into the caller's buffer and returns that buffer.
static float* __cdecl Collision_ScaleDirection(float* scaledOut, float scale, float* direction) {
    scaledOut[0] = scale * direction[0];
    scaledOut[1] = scale * direction[1];
    scaledOut[2] = scale * direction[2];
    return scaledOut;
}

HOOK(0x00401000, Collision_ScaleDirection, "auto");
