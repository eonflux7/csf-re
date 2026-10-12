#include "hook.h"

// 0x00402000: float* __thiscall AiStatic_AddVectors(float* this, float* a2, float* a3)
// Writes a2[i] = this[i] + a3[i] for i in 0..2 and returns a2.
static float* __fastcall AiStatic_AddVectors(float* self, void* /*edx*/, float* outVector, float* addVector) {
    float sumX = addVector[0] + self[0];
    float sumY = addVector[1] + self[1];
    float sumZ = addVector[2] + self[2];

    outVector[0] = sumX;
    outVector[1] = sumY;
    outVector[2] = sumZ;
    return outVector;
}

HOOK(0x402000, AiStatic_AddVectors, "auto");
