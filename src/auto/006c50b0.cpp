#include <cmath>
#include "globals.h"
#include "hook.h"

// Distance from the object's transform position (a1->+0x24 -> +0x30) to the
// position reported by the virtual getter chain on a2.
static double __cdecl GfxSkin_ComputeDistanceToTarget(void* a1, void* a2) {
    if (!a1 || !a2) {
        CSF_GLOBAL(float, g_fZero, 0x82D4CC);
        return g_fZero;
    }

    // a2->vtbl[4]() returns an object; that object's vtbl[4]() yields the target position block.
    void* targetObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[4])(a2, nullptr);
    float* targetPos = (float*)((void* (__fastcall*)(void*, void*))CSF_AT(void**, targetObj, 0)[4])(targetObj, nullptr);

    // Own position: pointer stored at a1+0x24, position block at +0x30.
    float* ownPos = reinterpret_cast<float*>(CSF_AT(uint8_t*, a1, 0x24) + 0x30);

    float deltaX = ownPos[0] - targetPos[0];
    float deltaY = ownPos[1] - targetPos[1];
    float deltaZ = ownPos[2] - targetPos[2];

    return std::sqrt(deltaZ * deltaZ + deltaX * deltaX + deltaY * deltaY);
}

HOOK(0x6C50B0, GfxSkin_ComputeDistanceToTarget, "auto");
