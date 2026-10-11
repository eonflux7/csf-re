#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Transforms the 3D vector pointed to by `in` by the matrix at `this`, writes the
// result to `out` (3 dwords), and returns `out`.
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));

static uint32_t* __fastcall S_00526d40_TransformVector3(S_00526d40* self, void* /*edx*/, uint32_t* out, int* in) {
    int transformed[3] = {0, 0, 0};
    int inputComponent = *in;
    RwV3dTransformVectors(reinterpret_cast<int>(&transformed[0]),
                          reinterpret_cast<int>(self),
                          1,
                          inputComponent);
    out[0] = static_cast<uint32_t>(transformed[0]);
    out[1] = static_cast<uint32_t>(transformed[1]);
    out[2] = static_cast<uint32_t>(transformed[2]);
    return out;
}

HOOK(0x004069F0, S_00526d40_TransformVector3, "auto");
