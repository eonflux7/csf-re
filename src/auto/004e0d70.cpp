#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Plane-hit callee at 0x004E0C20. Its ecx (self) is the caller's untouched
// incoming ecx; the original never loads ecx before the call.
CSF_FUNC(char, __fastcall, S_004e0c20_TryClipPlaneToRay, 0x004E0C20,
         (void* self, void* edx, uint32_t* a2, float* a3));

CSF_FUNC(double, __fastcall, GfxPlane_RayIntersectionT, 0x006AD8C0,
         (float* self, void* edx, float* rayOrigin, float* rayDirection));

// Returns the ray-plane hit parameter when the plane test succeeds,
// otherwise the sentinel 100000000.0f.
//
// The original keeps the plane in a 0x30-byte frame slot at [ebp-0x30]: ten
// floats (0x00..0x24), then the dirty flag byte at +0x28 (= [ebp-8]). The
// callee 0x6AD8C0 tests that byte (`*((_BYTE *)this + 40)`), so the flag must
// sit right after the floats, not in a separate local.
static double __fastcall S_004e0c20_GetRayPlaneHitT(void* self, void* edx,
                                                    uint32_t* a2, int a3)
{
    float v6 = 100000000.0f;
    float v4[12];
    std::memset(v4, 0, 12);
    v4[3] = 1.0f;
    v4[4] = 0.0f;
    v4[5] = 0.0f;
    CSF_AT(uint8_t, v4, 0x28) = 1;

    if (S_004e0c20_TryClipPlaneToRay(self, edx, a2, v4))
    {
        return GfxPlane_RayIntersectionT(v4, edx,
                                         reinterpret_cast<float*>(a2),
                                         reinterpret_cast<float*>(static_cast<uintptr_t>(a3)));
    }
    return static_cast<double>(v6);
}

HOOK(0x004E0D70, S_004e0c20_GetRayPlaneHitT, "auto");
