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

CSF_FUNC(int, __cdecl, RpAtomicGetWorldBoundingSphere, 0x0058EC60, (int a1));
CSF_FUNC(char, __fastcall, Sphere_GrowList, 0x006C0990, (void* self, void* edx, int a2));

// Appends the world bounding sphere (x, y, z, radius) of an RpAtomic to a
// growable 16-byte-entry list: [0]=data ptr, [4]=unused here, [8]=count,
// [0x0C]=grow amount. Returns the atomic.
static int __cdecl GfxFrames_AppendAtomicBoundingSphere(int atomic, void* list)
{
    int* sphere = reinterpret_cast<int*>(RpAtomicGetWorldBoundingSphere(atomic));
    int centerX = sphere[0];
    int centerY = sphere[1];
    int centerZ = sphere[2];
    int radius = sphere[3];

    if (CSF_AT(uint32_t, list, 0x08) >= CSF_AT(uint32_t, list, 0x04)) {
        Sphere_GrowList(list, nullptr, CSF_AT(int, list, 0x0C));
    }

    uint32_t count = CSF_AT(uint32_t, list, 0x08);
    int* entry = reinterpret_cast<int*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, list, 0x00)) + 16u * count);
    CSF_AT(uint32_t, list, 0x08) = count + 1;

    entry[0] = centerX;
    entry[1] = centerY;
    entry[2] = centerZ;
    entry[3] = radius;
    return atomic;
}

HOOK(0x006C0B20, GfxFrames_AppendAtomicBoundingSphere, "auto");
