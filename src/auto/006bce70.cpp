#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

using IntersectionCallback = int(__cdecl*)(int, int*, int);

CSF_FUNC(std::uint32_t*, __cdecl, GfxFrames_ComputeClumpLocalBoundingSphere, 0x006C0D70, (std::uint32_t* result, int clump));
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectorIntersections, 0x0056B9B0, (int world, float* sphere, IntersectionCallback callback, int userData));

// Tests whether a clump's bounding sphere (moved by the clump's frame translation)
// intersects any world sector. Returns the byte set by the intersection callback.
static char __cdecl GfxEnvironment_IsClumpInHierarchy(int clump, int world)
{
    // Local bounding sphere of the clump (center xyz + radius), 7 dwords of stack.
    alignas(8) float boundsStorage[7];
    GfxFrames_ComputeClumpLocalBoundingSphere(reinterpret_cast<std::uint32_t*>(boundsStorage), clump);

    // Frame of the clump; translation lives at frame + 0x10 + 0x30.
    void* clumpPtr = reinterpret_cast<void*>(static_cast<std::uintptr_t>(clump));
    std::uint8_t* frame = CSF_AT(std::uint8_t*, clumpPtr, 4);

    // Sphere in world space, laid out as the sector query expects.
    alignas(8) std::uint32_t sphereStorage[7];
    float* sphere = reinterpret_cast<float*>(sphereStorage);
    sphere[0] = boundsStorage[0] + CSF_AT(float, frame, 0x40);
    sphere[1] = boundsStorage[1] + CSF_AT(float, frame, 0x44);
    sphere[2] = boundsStorage[2] + CSF_AT(float, frame, 0x48);
    sphere[3] = boundsStorage[3];
    sphereStorage[6] = 3;

    std::uint8_t hitFlag = 0;
    RpWorldForAllWorldSectorIntersections(
        world,
        sphere,
        reinterpret_cast<IntersectionCallback>(CSF_ADDR_FN(0x006BC660)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&hitFlag)));

    return static_cast<char>(hitFlag);
}

HOOK(0x006BCE70, GfxEnvironment_IsClumpInHierarchy, "auto");
