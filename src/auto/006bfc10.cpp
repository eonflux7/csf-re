#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls (bound by address).
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));

// Transforms one 3D point (a3 = caller's source point address) by the frame's LTM
// matrix (frame = self->field_000c) and writes the result into the caller's output triple.
// Returns the output triple.
static uint32_t* __fastcall C_00860700_TransformPointsByFrameLTM(
    C_00860700* self, void* /*edx*/, uint32_t* outPoint, int sourcePointAddress)
{
    std::uint32_t* frameLtm = RwFrameGetLTM(self->field_000c);

    int transformed[3] = {0, 0, 0};
    RwV3dTransformPoints(reinterpret_cast<int>(&transformed[0]),
                         sourcePointAddress,
                         1,
                         reinterpret_cast<int>(frameLtm));

    outPoint[0] = static_cast<uint32_t>(transformed[0]);
    outPoint[1] = static_cast<uint32_t>(transformed[1]);
    outPoint[2] = static_cast<uint32_t>(transformed[2]);
    return outPoint;
}

HOOK(0x006BFC10, C_00860700_TransformPointsByFrameLTM, "auto");
