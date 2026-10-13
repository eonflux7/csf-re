#include <cstdint>
#include <cstddef>
#include "hook.h"

// Compacts the clip triangle vertex list in place. Entries whose v[1] <= a2 (NaN-safe)
// are kept: x and y are copied, z is copied and clamped to a2 when it exceeds it.
// The end pointer at a1[114] is then reset to just past the kept entries.
// Returns the end pointer as it was when the loop last read it.
static unsigned int __cdecl GfxCollision_ClipTriangleQuery(uint32_t* a1, float a2) {
    unsigned int result = a1[114];
    float* v5 = reinterpret_cast<float*>(a1 + 15);
    int keptCount = 0;

    if (reinterpret_cast<uintptr_t>(v5) < result) {
        float* v7 = v5;
        float* v8 = reinterpret_cast<float*>(a1 + 17);
        do {
            if (v5[1] <= a2) {
                float* v9 = v8 - 2;
                v9[0] = v7[0];
                v9[1] = v7[1];
                v8[0] = v7[2];
                if (v8[0] > a2) {
                    v8[0] = a2;
                }
                ++keptCount;
                v8 += 3;
            }
            result = a1[114];
            v5 += 3;
            v7 = v5;
        } while (reinterpret_cast<uintptr_t>(v5) < result);
    }

    a1[114] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&a1[3 * keptCount + 15]));
    return result;
}

HOOK(0x007C2870, GfxCollision_ClipTriangleQuery, "auto");
