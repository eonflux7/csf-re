#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(int, __fastcall, S_00402350_IntersectOrientedBox, 0x006ACB20, (S_00402350* self, void* , const void* box, float* firstDistance, float* secondDistance));

static bool __fastcall Collision_IntersectIndexedBounds(
    void* self, void* /*edx*/, uint32_t boundsIndex, const void* query)
{
    // The packet labels these call targets as vtable references.
    (void)CSF_VTBL(0x00402350);
    (void)CSF_VTBL(0x006ABB80);

    auto* records = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, self, 0x0C));
    auto* boundsData = records + (40 * boundsIndex + 16);

    alignas(8) uint8_t boundsBytes[0x3C];
    alignas(8) uint8_t queryBytes[0x18];
    alignas(8) uint8_t firstDistanceBytes[0x4];

    S00402350_InitTransform(
        reinterpret_cast<S_00402350*>(boundsBytes), nullptr);

    const float zSecond =
        CSF_AT(float, boundsData, 0x08) - CSF_AT(float, boundsData, 0x0C);
    const float zFirst =
        CSF_AT(float, boundsData, 0x0C) + CSF_AT(float, boundsData, 0x08);
    const float ySecond =
        CSF_AT(float, boundsData, 0x10) + CSF_AT(float, boundsData, 0x04);
    const float yFirst = CSF_AT(float, boundsData, 0x04);
    const float xSecond =
        CSF_AT(float, boundsData, 0x00) - CSF_AT(float, boundsData, 0x0C);
    const float xFirst =
        CSF_AT(float, boundsData, 0x0C) + CSF_AT(float, boundsData, 0x00);

    S_006abb80_InitializeBounds(
        reinterpret_cast<S_006abb80*>(boundsBytes), nullptr,
        xFirst, xSecond, yFirst, ySecond, zFirst, zSecond);

    CSF_AT(uint32_t, queryBytes, 0x00) = CSF_AT(uint32_t, query, 0x40);
    CSF_AT(uint32_t, queryBytes, 0x04) = CSF_AT(uint32_t, query, 0x44);
    CSF_AT(uint32_t, queryBytes, 0x08) = CSF_AT(uint32_t, query, 0x48);
    CSF_AT(uint32_t, queryBytes, 0x0C) = CSF_AT(uint32_t, query, 0x34);
    CSF_AT(uint32_t, queryBytes, 0x10) = CSF_AT(uint32_t, query, 0x38);
    CSF_AT(uint32_t, queryBytes, 0x14) = CSF_AT(uint32_t, query, 0x3C);

    NormalizeVector3(reinterpret_cast<C_00833b88*>(queryBytes), nullptr);

    float secondDistance;
    std::memcpy(&secondDistance, &boundsIndex, sizeof(secondDistance));

    return S_00402350_IntersectOrientedBox(
        reinterpret_cast<S_00402350*>(queryBytes), nullptr,
        boundsBytes, reinterpret_cast<float*>(firstDistanceBytes),
        &secondDistance) != 0;
}

HOOK(0x00443BA0, Collision_IntersectIndexedBounds, "auto");
