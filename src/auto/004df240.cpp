#include <cmath>
#include <cstdint>

#include "globals.h"
#include "hook.h"

// 0x82D4D4: float constant (1.0) used as the numerator of the reciprocal.
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);
// 0x83E138: float scale (1e8) applied to |denominator| for the parallel/bounds test.
CSF_GLOBAL(float, g_ParallelEpsilonScale, 0x83E138);

// Solves P + t*D = Q + s*E in 2D (P = (a1,a2), D = (a3,a4), Q = (a5,a6), E = (a7,a8)).
// Writes the two parameters through outA / outB and returns 1, or returns 0 when the
// lines are (nearly) parallel or the solution falls outside the accepted range.
static int __cdecl IntersectLines2DParams(float p1x, float p1y, float dirX, float dirY,
                                          float q1x, float q1y, float e1x, float e1y,
                                          float* outA, float* outB) {
    // Cross product of the two direction vectors: D.y*E.x - D.x*E.y.
    float denom = dirY * e1x - dirX * e1y;

    float diffX = q1x - p1x;
    float diffY = q1y - p1y;

    // Cross products of (Q - P) with the directions.
    float numA = e1x * diffY - diffX * e1y;
    float numB = diffY * dirX - diffX * dirY;

    float limit = std::fabs(denom) * g_ParallelEpsilonScale;
    if (!(std::fabs(numA) < limit && std::fabs(numB) < limit)) {
        return 0;
    }

    float inv = g_TransitionOne / denom;
    *outA = numA * inv;
    *outB = inv * numB;
    return 1;
}

HOOK(0x004DF240, IntersectLines2DParams, "auto");
