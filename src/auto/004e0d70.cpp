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
static double __fastcall S_004e0c20_GetRayPlaneHitT(void* self, void* /*edx*/,
                                                    uint32_t* a2, int a3)
{
    float v6 = 100000000.0f;
    float v4[10];
    std::memset(v4, 0, 12);
    v4[3] = 1.0f;
    v4[4] = 0.0f;
    v4[5] = 0.0f;
    uint8_t v5 = 1;
    (void)v5;

    if (S_004e0c20_TryClipPlaneToRay(self, nullptr, a2, v4))
    {
        return GfxPlane_RayIntersectionT(v4, nullptr,
                                         reinterpret_cast<float*>(a2),
                                         reinterpret_cast<float*>(static_cast<uintptr_t>(a3)));
    }
    return static_cast<double>(v6);
}

HOOK(0x004E0D70, S_004e0c20_GetRayPlaneHitT, "auto");
