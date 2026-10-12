#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall GfxPtank_TestBoundaryFace(
    int* self, void* /*edx*/, int faceIndex, float* point)
{
    void* face = CSF_AT(void**, self, 0x10)[faceIndex];
    const int vertexCount = CSF_AT(int, face, 0x8);

    for (int edge = 0; edge < vertexCount; ++edge)
    {
        const int currentIndex = CSF_AT(int*, face, 0x0)[edge];
        const float* current = reinterpret_cast<const float*>(
            CSF_AT(std::uint8_t*, self, 0x0) + 8 * currentIndex);

        const int nextIndex =
            CSF_AT(int*, face, 0x0)[(edge + 1) % vertexCount];
        const float* next = reinterpret_cast<const float*>(
            CSF_AT(std::uint8_t*, self, 0x0) + 8 * nextIndex);

        const float cross =
            (point[0] - next[0]) * (next[1] - current[1]) -
            (point[1] - next[1]) * (next[0] - current[0]);

        if (!(cross < g_fZero))
            return 0;
    }

    return 1;
}

HOOK(0x006D7450, GfxPtank_TestBoundaryFace, "auto");
