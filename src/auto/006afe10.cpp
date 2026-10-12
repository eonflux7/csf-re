#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Geometry_IntersectSegments, 0x004DEFD0,
         (float a1, float a2, float a3, float a4,
          float a5, float a6, float a7, float a8,
          float* a9, float* a10));

static bool __cdecl Geometry_IsPolygonSimple(std::uint32_t* polygon)
{
    const std::uint32_t vertexCount = CSF_AT(std::uint32_t, polygon, 0x08);
    if (vertexCount == 1)
        return true;

    float* vertices = CSF_AT(float*, polygon, 0x00);
    float* currentY = vertices + 1;
    std::uint32_t edgeIndex = 0;

    alignas(8) std::uint8_t firstOutputBytes[0x04];
    alignas(8) std::uint8_t secondOutputBytes[0x0C];

    do
    {
        const float directionX = currentY[1] - currentY[-1];
        const float directionY = currentY[2] - currentY[0];

        std::uint32_t otherEdge = edgeIndex + 2;
        std::uint32_t nextVertex = edgeIndex + 3;

        while (otherEdge < vertexCount)
        {
            const std::uint32_t wrappedVertex = nextVertex % vertexCount;
            const float otherDirectionX =
                vertices[2 * wrappedVertex] - vertices[2 * otherEdge];
            const float otherDirectionY =
                vertices[2 * wrappedVertex + 1] - vertices[2 * otherEdge + 1];

            if (Geometry_IntersectSegments(
                    currentY[-1], currentY[0], directionX, directionY,
                    vertices[2 * otherEdge], vertices[2 * otherEdge + 1],
                    otherDirectionX, otherDirectionY,
                    reinterpret_cast<float*>(firstOutputBytes),
                    reinterpret_cast<float*>(secondOutputBytes)))
            {
                return false;
            }

            ++otherEdge;
            ++nextVertex;
        }

        ++edgeIndex;
        currentY += 2;
    } while (edgeIndex < vertexCount - 1);

    return true;
}

HOOK(0x006AFE10, Geometry_IsPolygonSimple, "auto");
