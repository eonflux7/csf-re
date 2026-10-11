#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetProviderPosition, 0x004F4EB0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the spawn-point provider position as a flat (XZ) direction vector,
// normalized when both X and Z are nonzero. Result is written to the caller's
// buffer at a1 (x, y = 0, z) and a1 is returned.
static int __stdcall MpSpawnPoints_GetNormalizedFlatPosition(int a1) {
    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    void* cachedProvider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
    int providerPosition = MpSpawnPoints_GetProviderPosition(cachedProvider, nullptr);

    // Stack layout mirrors the original: vec[0]=x, vec[1]=y (forced to 0), vec[2]=z.
    const float* providerVec =
        reinterpret_cast<const float*>(static_cast<uintptr_t>(providerPosition));
    float vec[3];
    vec[0] = providerVec[0];
    vec[1] = 0.0f;
    vec[2] = providerVec[2];

    if (vec[0] != g_fZero && vec[2] != g_fZero)
        NormalizeVector3(vec, nullptr);

    float* out = reinterpret_cast<float*>(static_cast<uintptr_t>(a1));
    out[0] = vec[0];
    out[1] = vec[1];
    out[2] = vec[2];
    return a1;
}

HOOK(0x00411E70, MpSpawnPoints_GetNormalizedFlatPosition, "auto");
