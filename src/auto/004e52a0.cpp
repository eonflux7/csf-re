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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* edx, float* out));
CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegToRad, 0x0083E9A8);

// cos(r) for |r| <= pi/4
static double CosKernel(double r)
{
    const double r2 = r * r;
    double term = 1.0;
    double sum = 1.0;
    for (int i = 1; i <= 10; ++i)
    {
        term *= -r2 / (double)((2 * i - 1) * (2 * i));
        sum += term;
    }
    return sum;
}

// sin(r) for |r| <= pi/4
static double SinKernel(double r)
{
    const double r2 = r * r;
    double term = r;
    double sum = r;
    for (int i = 1; i <= 10; ++i)
    {
        term *= -r2 / (double)((2 * i) * (2 * i + 1));
        sum += term;
    }
    return sum;
}

// The original computes the cosine with the processor's FCOS instruction (no CRT call).
// This is a self-contained cosine with the same domain behaviour: operands with
// |x| >= 2^63 are returned unchanged, infinities/NaN give NaN, and everything else is
// reduced exactly (integer long division) by pi/2 using the 66-bit pi the FPU uses.
static double Cosine(double x)
{
    if (x != x)
        return x;

    const double ax = x < 0.0 ? -x : x;
    if (ax >= 0x1p63)
    {
        if (ax > DBL_MAX)
            return x - x;       // infinity: invalid operand -> NaN
        return x;               // out of range: operand left untouched
    }

    if (ax < 0.78539816339744830962)
        return CosKernel(ax);

    // ax = (intPart * 2^53 + fracBits) * 2^-53, both parts exact.
    const int64_t intPart = (int64_t)ax;
    const double fraction = ax - (double)intPart;
    const int64_t fracBits = (int64_t)(fraction * 0x1p53);

    // pi/2 = 0xC90FDAA22168C234C * 2^-67
    const uint32_t kHalfPiHi = 0xCu;
    const uint64_t kHalfPiLo = 0x90FDAA22168C234CULL;

    // Dividend in units of 2^-67: the 116 bits above followed by 14 zero bits.
    const uint32_t chunkBits[5] = {
        (uint32_t)((uint64_t)intPart >> 32),
        (uint32_t)intPart,
        (uint32_t)((uint64_t)fracBits >> 32),
        (uint32_t)fracBits,
        0u
    };
    const int chunkCount[5] = { 31, 32, 21, 32, 14 };

    uint32_t remHi = 0;
    uint64_t remLo = 0;
    uint32_t quadrant = 0;

    for (int chunk = 0; chunk < 5; ++chunk)
    {
        for (int bit = chunkCount[chunk] - 1; bit >= 0; --bit)
        {
            const uint32_t incoming = (chunkBits[chunk] >> bit) & 1u;
            remHi = (remHi << 1) | (uint32_t)(remLo >> 63);
            remLo = (remLo << 1) | incoming;
            quadrant <<= 1;
            if (remHi > kHalfPiHi || (remHi == kHalfPiHi && remLo >= kHalfPiLo))
            {
                remHi -= kHalfPiHi + (remLo < kHalfPiLo ? 1u : 0u);
                remLo -= kHalfPiLo;
                quadrant |= 1u;
            }
        }
    }

    // Fold the remainder into [-pi/4, pi/4].
    bool negative = false;
    const uint64_t foldLo = kHalfPiLo - remLo;
    const uint32_t foldHi = kHalfPiHi - remHi - (kHalfPiLo < remLo ? 1u : 0u);
    if (remHi > foldHi || (remHi == foldHi && remLo > foldLo))
    {
        remHi = foldHi;
        remLo = foldLo;
        negative = true;
        ++quadrant;
    }

    double reduced = ((double)(int64_t)remHi * 0x1p64
                    + (double)(int64_t)(uint32_t)(remLo >> 32) * 0x1p32
                    + (double)(int64_t)(uint32_t)remLo) * 0x1p-67;
    if (negative)
        reduced = -reduced;

    switch (quadrant & 3u)
    {
    case 0:  return CosKernel(reduced);
    case 1:  return -SinKernel(reduced);
    case 2:  return -CosKernel(reduced);
    default: return SinKernel(reduced);
    }
}

// Checks whether the target object lies inside the horizontal view cone of this object;
// also caches cos(halfAngle) in field_0040.
static bool __fastcall C_0083eb70_IsTargetInCone(C_0083eb70* self, void* /*edx*/, int targetHandle)
{
    void* target = reinterpret_cast<void*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));
    if (!target)
        return false;

    float direction[3];             // [ebp-0x28]
    alignas(4) uint8_t posTemp[4];  // [ebp-0x1C]
    alignas(8) uint8_t anglesTemp[8]; // [ebp-0x18]
    float forward[2];               // [ebp-0x10]
    float toTarget[2];              // [ebp-0x08]

    // vtable[0x8C / 4]: returns the orientation, converted into a direction vector
    float* angles = ((float*(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0x8C / 4])(
        self, nullptr, anglesTemp);
    GfxMatrix_EulerAnglesToDirection(angles, nullptr, direction);

    forward[0] = direction[0];
    forward[1] = direction[2];
    NormalizeVector2(forward, nullptr);

    // vtable[0x88 / 4]: own position
    float* ownPos = ((float*(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0x88 / 4])(
        self, nullptr, posTemp);

    float* targetPos = GetObjectPositionVector(target, nullptr);

    toTarget[0] = targetPos[0] - ownPos[0];
    toTarget[1] = targetPos[2] - ownPos[2];
    NormalizeVector2(toTarget, nullptr);

    float dot = toTarget[1] * forward[1] + toTarget[0] * forward[0];

    double halfAngleDeg = self->vSlot7C();
    double coneCosine = Cosine(halfAngleDeg * g_DegToRad);
    self->field_0040 = (float)coneCosine;

    return dot >= coneCosine;
}

HOOK(0x004E52A0, C_0083eb70_IsTargetInCone, "auto");
