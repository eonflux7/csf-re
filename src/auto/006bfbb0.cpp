#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls (bound by address).
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));

// Transforms one 3D vector by the frame's LTM matrix (frame = self->field_000c)
// and writes the result into the caller's output triple.
static uint32_t* __fastcall C_00860700_TransformVectorsByFrameLTM(
    C_00860700* self, void* /*edx*/, uint32_t* outVector, uint32_t* sourceVector)
{
    std::uint32_t* frameLtm = RwFrameGetLTM(self->field_000c);

    int transformed[3] = {0, 0, 0};
    RwV3dTransformVectors(reinterpret_cast<int>(&transformed[0]),
                          reinterpret_cast<int>(sourceVector),
                          1,
                          reinterpret_cast<int>(frameLtm));

    outVector[0] = static_cast<uint32_t>(transformed[0]);
    outVector[1] = static_cast<uint32_t>(transformed[1]);
    outVector[2] = static_cast<uint32_t>(transformed[2]);
    return outVector;
}

HOOK(0x006BFBB0, C_00860700_TransformVectorsByFrameLTM, "auto");
