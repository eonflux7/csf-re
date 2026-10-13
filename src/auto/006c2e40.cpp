#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

// Record layout (a1): +0 keyframe A ptr, +4 keyframe B ptr, +8 query time (float),
// +12/+16/+20 interpolated output. Keyframe: [0]=time, [1..3]=values.
static float* __stdcall GfxAnimCompress_InterpolateAtTime(uint32_t a1)
{
    void* rec = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    float* keyA = CSF_AT(float*, rec, 0);
    float* keyB = CSF_AT(float*, rec, 4);

    if (keyA == keyB)
    {
        // Degenerate pair: copy the sample values of A directly.
        float* sample = keyA + 1;
        CSF_AT(float, rec, 12) = sample[0];
        CSF_AT(float, rec, 16) = sample[1];

        // The third value is stored and returned as its raw dword bits.
        uint32_t rawBits = 0;
        std::memcpy(&rawBits, &sample[2], sizeof(rawBits));
        CSF_AT(uint32_t, rec, 20) = rawBits;
        return reinterpret_cast<float*>(static_cast<uintptr_t>(rawBits));
    }

    // Linear interpolation of the three values by the normalized time.
    float ratio = (CSF_AT(float, rec, 8) - keyA[0]) / (keyB[0] - keyA[0]);
    CSF_AT(float, rec, 12) = (keyB[1] - keyA[1]) * ratio + keyA[1];
    CSF_AT(float, rec, 16) = (keyB[2] - keyA[2]) * ratio + keyA[2];
    CSF_AT(float, rec, 20) = (keyB[3] - keyA[3]) * ratio + keyA[3];
    return keyA;
}

HOOK(0x006C2E40, GfxAnimCompress_InterpolateAtTime, "auto");
