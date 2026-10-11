#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(float*, __fastcall, ProjectPointOntoSegment, 0x006B1240, (void* self, void* edx, float* a2, float* a3));

static float* __cdecl GfxMatrix_FindClosestPointOnTriangleEdges(
    float* result, float* point,
    float* vertexA, float* vertexB, float* vertexC)
{
    float edgeAB[6] = {
        vertexA[0], vertexA[1], vertexA[2],
        vertexB[0], vertexB[1], vertexB[2]
    };
    float edgeBC[6] = {
        vertexB[0], vertexB[1], vertexB[2],
        vertexC[0], vertexC[1], vertexC[2]
    };
    float edgeCA[6] = {
        vertexC[0], vertexC[1], vertexC[2],
        vertexA[0], vertexA[1], vertexA[2]
    };

    float projectionAB[3];
    float projectionBC[3];
    float projectionCA[3];

    ProjectPointOntoSegment(edgeAB, nullptr, projectionAB, point);
    ProjectPointOntoSegment(edgeBC, nullptr, projectionBC, point);
    ProjectPointOntoSegment(edgeCA, nullptr, projectionCA, point);

    const float abX = point[0] - projectionAB[0];
    const float abY = point[1] - projectionAB[1];
    const float abZ = point[2] - projectionAB[2];

    const float bcX = point[0] - projectionBC[0];
    const float bcY = point[1] - projectionBC[1];
    const float bcZ = point[2] - projectionBC[2];
    const float distanceBC = bcZ * bcZ + bcY * bcY + bcX * bcX;

    const float caX = point[0] - projectionCA[0];
    const float caY = point[1] - projectionCA[1];
    const float caZ = point[2] - projectionCA[2];
    const float distanceCA = caZ * caZ + caY * caY + caX * caX;

    float nearestDistance = abZ * abZ + abY * abY + abX * abX;
    const float* nearest = projectionAB;

    if (distanceBC < nearestDistance) {
        nearest = projectionBC;
        nearestDistance = distanceBC;
    }
    if (distanceCA < nearestDistance) {
        nearest = projectionCA;
    }

    result[0] = nearest[0];
    result[1] = nearest[1];
    result[2] = nearest[2];
    return result;
}

HOOK(0x006AD320, GfxMatrix_FindClosestPointOnTriangleEdges, "auto");
