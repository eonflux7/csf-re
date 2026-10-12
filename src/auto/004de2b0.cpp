#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Geometry_IntersectSegmentRectangle, 0x004DDD90,
         (float a1, float a2, float a3, float a4,
          float a5, float a6, float a7, float a8));
CSF_FUNC(int, __fastcall, C_0086709c_GetVertex, 0x004E08C0,
         (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, C_0086709c_ContainsPoint, 0x004E0A70,
         (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, C_0086709c_GetIndexedElement, 0x00732150,
         (void* self, void* edx, int a2));

static char __stdcall C_0086709c_IntersectsRectangle(
    S_004e0a70* minimum, S_004e0a70* maximum, C_0086709c* polygon)
{
    // The packet also lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x004E0A70);

    alignas(8) uint8_t lowerRightBytes[0x8];
    alignas(8) uint8_t upperLeftBytes[0x8];

    CSF_AT(float, lowerRightBytes, 0x0) = CSF_AT(float, maximum, 0x0);
    CSF_AT(float, lowerRightBytes, 0x4) = CSF_AT(float, minimum, 0x4);
    CSF_AT(float, upperLeftBytes, 0x0) = CSF_AT(float, minimum, 0x0);
    CSF_AT(float, upperLeftBytes, 0x4) = CSF_AT(float, maximum, 0x4);

    char intersects =
        C_0086709c_ContainsPoint(polygon, nullptr, reinterpret_cast<float*>(minimum)) ||
        C_0086709c_ContainsPoint(polygon, nullptr, reinterpret_cast<float*>(maximum)) ||
        C_0086709c_ContainsPoint(polygon, nullptr, reinterpret_cast<float*>(lowerRightBytes)) ||
        C_0086709c_ContainsPoint(polygon, nullptr, reinterpret_cast<float*>(upperLeftBytes));

    for (uint32_t index = 0; !intersects; ++index) {
        if (index >= CSF_AT(uint32_t, polygon, 0x8))
            break;

        auto* vertex = reinterpret_cast<float*>(
            C_0086709c_GetVertex(polygon, nullptr, static_cast<int>(index)));
        const float x = CSF_AT(float, vertex, 0x0);
        const float z = CSF_AT(float, vertex, 0x8);

        intersects =
            x >= CSF_AT(float, minimum, 0x0) &&
            x < CSF_AT(float, maximum, 0x0) &&
            z >= CSF_AT(float, minimum, 0x4) &&
            z < CSF_AT(float, maximum, 0x4);
    }

    for (uint32_t index = 0; !intersects; ++index) {
        if (index >= CSF_AT(uint32_t, polygon, 0x8))
            break;

        C_0086709c_GetIndexedElement(polygon, nullptr, static_cast<int>(index));

        auto* vertex = reinterpret_cast<float*>(
            C_0086709c_GetVertex(polygon, nullptr, static_cast<int>(index)));
        const float x = CSF_AT(float, vertex, 0x0);
        const float z = CSF_AT(float, vertex, 0x8);

        const uint32_t nextIndex =
            index == CSF_AT(uint32_t, polygon, 0x8) - 1 ? 0 : index + 1;
        auto* nextVertex = reinterpret_cast<float*>(
            C_0086709c_GetVertex(polygon, nullptr, static_cast<int>(nextIndex)));

        const float deltaX = CSF_AT(float, nextVertex, 0x0) - x;
        const float deltaZ = CSF_AT(float, nextVertex, 0x8) - z;

        intersects = Geometry_IntersectSegmentRectangle(
            x, z, deltaX, deltaZ,
            CSF_AT(float, minimum, 0x0),
            CSF_AT(float, minimum, 0x4),
            CSF_AT(float, maximum, 0x0),
            CSF_AT(float, maximum, 0x4)) != 0;
    }

    return intersects;
}

HOOK(0x004DE2B0, C_0086709c_IntersectsRectangle, "auto");
