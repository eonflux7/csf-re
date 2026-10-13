#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, PlaneDistanceToPoint, 0x004024D0, (float* self, void* , float* point));
CSF_FUNC(int, __fastcall, GfxMatrix_ClassifyPointAgainstPlane, 0x006AD7B0, (float* self, void* , float* point));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall CCamera_TestBoxAgainstFrustum(
    CCamera* self, void* /*edx*/, float* referencePoint,
    float* boxVertices, char flags)
{
    alignas(8) uint8_t planeBytes[0x3C] = {};
    float* plane = reinterpret_cast<float*>(planeBytes);
    plane[3] = 1.0f;
    CSF_AT(uint8_t, planeBytes, 0x28) = 1;

    const bool firstPositive = PlaneDistanceToPoint(
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x74),
        nullptr, referencePoint) > g_fZero;

    const double secondDistance = PlaneDistanceToPoint(
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0xA0),
        nullptr, referencePoint);

    self->field_00d4 = firstPositive ? 4 : 2;
    self->field_00d8 = secondDistance > g_fZero ? 3 : 5;

    for (uint32_t selection = flags & 1; selection < 4; ++selection)
    {
        uint32_t planeIndex;
        switch (selection)
        {
        case 0:
            planeIndex = CSF_AT(uint32_t, self, 0xCC);
            break;
        case 1:
            planeIndex = CSF_AT(uint32_t, self, 0xD0);
            break;
        case 2:
            planeIndex = self->field_00d4;
            break;
        default:
            planeIndex = self->field_00d8;
            break;
        }

        uint8_t* planeData = reinterpret_cast<uint8_t*>(self->field_000c);
        const float distance = CSF_AT(float, planeData, planeIndex * 20 + 0xA0);
        float* normal = reinterpret_cast<float*>(
            planeData + planeIndex * 20 + 0x94);

        plane[0] = distance * normal[0];
        plane[1] = distance * normal[1];
        plane[2] = distance * normal[2];
        plane[3] = normal[0];
        plane[4] = normal[1];
        plane[5] = normal[2];
        CSF_AT(uint8_t, planeBytes, 0x28) = 1;

        float* vertex = boxVertices;
        uint32_t rejectedCount = 0;
        bool allRejected = true;
        while (allRejected)
        {
            if (GfxMatrix_ClassifyPointAgainstPlane(plane, nullptr, vertex))
            {
                allRejected = false;
            }
            else
            {
                ++rejectedCount;
                vertex += 3;
            }

            if (rejectedCount >= 8)
            {
                if (allRejected)
                    return 0;
                break;
            }
        }
    }

    return 1;
}

HOOK(0x006D35E0, CCamera_TestBoxAgainstFrustum, "auto");
