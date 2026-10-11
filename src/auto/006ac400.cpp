#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));

static S_00402350* __fastcall GfxMatrix_TransformFrame(
    uint32_t* self, void* /*edx*/, S_00402350* output, int* matrix)
{
    // sized by the original's frame (0x48 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t transformedBytes[0x48];
    S_00402350& transformed = *reinterpret_cast<S_00402350*>(transformedBytes);
    uint32_t vector[3];

    // The packet labels the initializer address as a vtable reference.
    (void)CSF_VTBL(0x00402350);
    InitS00402350Object(&transformed, nullptr);

    transformed.field_000c = CSF_AT(uint32_t, self, 0x0C);
    transformed.field_0010 = CSF_AT(uint32_t, self, 0x10);
    transformed.field_0014 = CSF_AT(uint32_t, self, 0x14);

    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    RwV3dTransformPoints(
        reinterpret_cast<int>(vector),
        reinterpret_cast<int>(self),
        1, *matrix);
    transformed.field_0000 = vector[0];
    transformed.field_0004 = vector[1];
    transformed.field_0008 = vector[2];

    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(vector),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x18),
        1, *matrix);
    CSF_AT(uint32_t, &transformed, 0x18) = vector[0];
    CSF_AT(uint32_t, &transformed, 0x1C) = vector[1];
    CSF_AT(uint32_t, &transformed, 0x20) = vector[2];

    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(vector),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24),
        1, *matrix);
    CSF_AT(uint32_t, &transformed, 0x24) = vector[0];
    CSF_AT(uint32_t, &transformed, 0x28) = vector[1];
    CSF_AT(uint32_t, &transformed, 0x2C) = vector[2];

    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(vector),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x30),
        1, *matrix);
    CSF_AT(uint32_t, &transformed, 0x30) = vector[0];
    CSF_AT(uint32_t, &transformed, 0x34) = vector[1];
    CSF_AT(uint32_t, &transformed, 0x38) = vector[2];

    std::memcpy(output, &transformed, 0x3C);
    return output;
}

HOOK(0x006AC400, GfxMatrix_TransformFrame, "auto");
