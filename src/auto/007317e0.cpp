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

// Reads a 4-float row from target (vtbl slot 27), scales its w component by
// scale * self->scaleFactor(+0x1C), writes the row back through vtbl slot 26,
// refreshes the target through vtbl slot 2, then writes the original row
// back through vtbl slot 26 again. Called with two stack arguments (target, scale).
static void __fastcall ApplyScaledAxisToTarget(float* self, void* /*edx*/, int* target, float scale)
{
    if (target == nullptr)
        return;

    // Stack block matching [ebp-0x20]: the copied row (x, y, z) and the original w.
    alignas(8) uint8_t frontBytes[0x10];
    // Stack block matching [ebp-0x10]: the row handed to the first write-back, scaled w at +0xC.
    alignas(8) uint8_t backBytes[0x10];

    int32_t* frontInts = reinterpret_cast<int32_t*>(frontBytes);
    float* frontFloats = reinterpret_cast<float*>(frontBytes);
    int32_t* backInts = reinterpret_cast<int32_t*>(backBytes);
    float* backFloats = reinterpret_cast<float*>(backBytes);

    // vtbl[27] (+0x6C): returns a pointer to a 4-dword row.
    uint8_t* rowPtr = ((uint8_t* (__fastcall*)(void*, void*))(CSF_AT(void**, target, 0)[27]))(target, nullptr);

    float weightedScale = scale * CSF_AT(float, self, 0x1C);

    frontInts[0] = CSF_AT(int32_t, rowPtr, 0x0);
    frontInts[1] = CSF_AT(int32_t, rowPtr, 0x4);
    frontInts[2] = CSF_AT(int32_t, rowPtr, 0x8);
    float rowW = CSF_AT(float, rowPtr, 0xC);
    frontFloats[3] = rowW;

    backInts[0] = frontInts[0];
    backInts[1] = frontInts[1];
    backFloats[3] = weightedScale * rowW;
    backInts[2] = frontInts[2];

    // vtbl[26] (+0x68): write the scaled row back.
    ((void (__fastcall*)(void*, void*, void*))(CSF_AT(void**, target, 0)[26]))(target, nullptr, backBytes);
    // vtbl[2] (+0x08): refresh.
    ((void (__fastcall*)(void*, void*))(CSF_AT(void**, target, 0)[2]))(target, nullptr);
    // vtbl[26] (+0x68): write the original row back.
    ((void (__fastcall*)(void*, void*, void*))(CSF_AT(void**, target, 0)[26]))(target, nullptr, frontBytes);
}

HOOK(0x7317E0, ApplyScaledAxisToTarget, "auto");
