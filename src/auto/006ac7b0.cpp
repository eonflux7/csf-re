#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (void* self, void* edx, float* a2));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static bool __fastcall S_006ac7b0_IntersectsSphere(
    S_006ac7b0* self, void* /*edx*/, int sphere, float* squaredDistance)
{
    // The packet labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x006ADA10);

    // Each matrix wrapper occupies eight bytes: a handle and an ownership byte.
    uint32_t translation[2] = {};
    uint32_t orientation[2] = {};
    translation[0] = reinterpret_cast<uint32_t>(RwMatrixCreate());
    CSF_AT(uint8_t, translation, 0x4) = 1;

    orientation[0] = reinterpret_cast<uint32_t>(RwMatrixCreate());
    float thirdRow[3];
    thirdRow[0] = -CSF_AT(float, self, 0x0);
    thirdRow[1] = -CSF_AT(float, self, 0x4);
    CSF_AT(uint8_t, orientation, 0x4) = 1;
    thirdRow[2] = -CSF_AT(float, self, 0x8);
    GfxMatrix_TranslateFrameByOffset(translation, nullptr, thirdRow);

    uint32_t firstRow[3];
    uint32_t secondRow[3];
    CSF_AT(uint32_t, thirdRow, 0x0) = CSF_AT(uint32_t, self, 0x20);
    CSF_AT(uint32_t, thirdRow, 0x4) = CSF_AT(uint32_t, self, 0x2C);
    CSF_AT(uint32_t, thirdRow, 0x8) = CSF_AT(uint32_t, self, 0x38);
    secondRow[0] = CSF_AT(uint32_t, self, 0x1C);
    secondRow[1] = CSF_AT(uint32_t, self, 0x28);
    firstRow[0] = CSF_AT(uint32_t, self, 0x18);
    secondRow[2] = CSF_AT(uint32_t, self, 0x34);
    firstRow[1] = CSF_AT(uint32_t, self, 0x24);
    firstRow[2] = CSF_AT(uint32_t, self, 0x30);

    GfxMatrix_SetFourRowsAndUpdate(
        orientation, nullptr, firstRow, secondRow,
        reinterpret_cast<uint32_t*>(thirdRow),
        reinterpret_cast<uint32_t*>(&g_ZeroVectorX));

    uint32_t* combined = RwMatrixCreate();
    RwMatrixMultiply(
        combined,
        reinterpret_cast<uint32_t*>(translation[0]),
        reinterpret_cast<uint32_t*>(orientation[0]));

    uint32_t* transform = RwMatrixCreate();
    std::memcpy(transform, combined, 0x40);
    RwMatrixDestroy(combined);

    secondRow[0] = 0;
    secondRow[1] = 0;
    secondRow[2] = 0;
    RwV3dTransformPoints(
        reinterpret_cast<int>(secondRow), sphere, 1,
        reinterpret_cast<int>(transform));

    std::memcpy(thirdRow, secondRow, 0xC);
    RwMatrixDestroy(transform);

    *squaredDistance = 0.0f;
    if (thirdRow[0] < -CSF_AT(float, self, 0xC)) {
        const float distance = thirdRow[0] + CSF_AT(float, self, 0xC);
        *squaredDistance = distance * distance;
    } else if (thirdRow[0] > CSF_AT(float, self, 0xC)) {
        const float distance = thirdRow[0] - CSF_AT(float, self, 0xC);
        *squaredDistance = distance * distance;
    }

    if (thirdRow[1] < -CSF_AT(float, self, 0x10)) {
        const float distance = thirdRow[1] + CSF_AT(float, self, 0x10);
        *squaredDistance = distance * distance + *squaredDistance;
    } else if (thirdRow[1] > CSF_AT(float, self, 0x10)) {
        const float distance = thirdRow[1] - CSF_AT(float, self, 0x10);
        *squaredDistance = distance * distance + *squaredDistance;
    }

    if (thirdRow[2] < -CSF_AT(float, self, 0x14)) {
        const float distance = thirdRow[2] + CSF_AT(float, self, 0x14);
        *squaredDistance = distance * distance + *squaredDistance;
    } else if (thirdRow[2] > CSF_AT(float, self, 0x14)) {
        const float distance = thirdRow[2] - CSF_AT(float, self, 0x14);
        *squaredDistance = distance * distance + *squaredDistance;
    }

    const bool intersects =
        *squaredDistance <=
        CSF_AT(float, reinterpret_cast<void*>(sphere), 0xC) *
        CSF_AT(float, reinterpret_cast<void*>(sphere), 0xC);

    if (CSF_AT(uint8_t, orientation, 0x4))
        RwMatrixDestroy(reinterpret_cast<uint32_t*>(orientation[0]));
    if (CSF_AT(uint8_t, translation, 0x4))
        RwMatrixDestroy(reinterpret_cast<uint32_t*>(translation[0]));

    return intersects;
}

HOOK(0x006AC7B0, S_006ac7b0_IntersectsSphere, "auto");
