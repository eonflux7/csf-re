#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* /*edx*/, uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_IntersectRaySphere_6ACDF0, 0x006ACDF0, (float* self, void* /*edx*/, float* sphere, float* nearDistance, float* farDistance));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall C_00863760_IntersectRayAndComputeHit(C_00863760* self, void* /*edx*/, float* ray, uint32_t* outHit, float distance)
{
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t outDirA[0xC];
    alignas(8) uint8_t outDirB[0xC];
    alignas(8) uint8_t dirBytes[0x18];

    uint32_t* sphere = reinterpret_cast<uint32_t*>(sphereBytes);
    float* sphereF = reinterpret_cast<float*>(sphereBytes);
    float* centre = &CSF_AT(float, self, 0x8);
    float farDistance = 0.0f;
    char result = 0;

    sphere[0] = self->field_0008;
    sphere[1] = self->field_000c;
    sphere[2] = self->field_0010;
    sphereF[3] = (distance + g_TransitionOne) * self->field_0014;

    int hits = GfxMatrix_IntersectRaySphere_6ACDF0(ray, nullptr, sphereF, &distance, &farDistance) - 1;

    float* out = reinterpret_cast<float*>(outHit);
    float* dir = reinterpret_cast<float*>(dirBytes);
    uint32_t* converted;

    if (hits == 0) {
        outHit[0] = reinterpret_cast<uint32_t*>(ray)[3];
        outHit[1] = reinterpret_cast<uint32_t*>(ray)[4];
        outHit[2] = reinterpret_cast<uint32_t*>(ray)[5];
        out[6] = 0.0f;
        dir[0] = out[0] - centre[0];
        dir[1] = out[1] - centre[1];
        dir[2] = out[2] - centre[2];
        converted = S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(dirBytes), nullptr,
                                                reinterpret_cast<uint32_t*>(outDirA));
    } else if (hits == 1) {
        float px = distance * ray[0];
        float py = distance * ray[1];
        float pz = distance * ray[2];
        float x = px + ray[3];
        float y = py + ray[4];
        float z = pz + ray[5];
        out[0] = x;
        out[1] = y;
        out[2] = z;
        out[6] = distance;
        dir[0] = out[0] - centre[0];
        dir[1] = out[1] - centre[1];
        dir[2] = out[2] - centre[2];
        converted = S_00526d40_ConvertDirection(reinterpret_cast<S_00526d40*>(dirBytes), nullptr,
                                                reinterpret_cast<uint32_t*>(outDirB));
    } else {
        return result;
    }

    outHit[3] = converted[0];
    outHit[4] = converted[1];
    outHit[5] = converted[2];
    return 1;
}

HOOK(0x0070BBE0, C_00863760_IntersectRayAndComputeHit, "auto");
