#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, sub_4DEFD0, 0x004DEFD0,
         (float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float* a9, float* a10));

static int __stdcall Geometry_SegmentsIntersectXZ(float* startA, float* endA, float* startB, float* endB,
                                                  float* outA, float* outB)
{
    float dirAx = endA[0] - startA[0];
    float dirAz = endA[2] - startA[2];
    float dirBx = endB[0] - startB[0];
    float dirBz = endB[2] - startB[2];

    return sub_4DEFD0(startA[0], startA[2], dirAx, dirAz,
                      startB[0], startB[2], dirBx, dirBz,
                      outA, outB) == 1;
}

HOOK(0x004DF0D0, Geometry_SegmentsIntersectXZ, "auto");
