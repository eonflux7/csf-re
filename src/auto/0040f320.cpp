#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_MatrixQueryFlags, 0x0082D6E8);
CSF_GLOBAL(float, g_RayLength, 0x0082DDDC);
CSF_GLOBAL(int, g_RayCollisionMask, 0x00869C5C);

CSF_FUNC(char, __fastcall, C_0082ddf8_UpdateRayHit, 0x0040EF40,
         (void* self, void* edx, float* a2, uint32_t* a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10,
         (std::uint32_t* matrix));

static char __fastcall C_0082ddf8_UpdateWorldRay(
    C_0082ddf8* self, void* /*edx*/)
{
    alignas(8) std::uint8_t matrixResult[0x8] = {};
    void* source = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, source, 0)[0x2C / 4])(
            source, nullptr, matrixResult, g_MatrixQueryFlags);

    float* matrix = CSF_AT(float*, matrixResult, 0);
    self->field_0044 = matrix[8];
    self->field_0048 = matrix[9];
    self->field_004c = matrix[10];

    float scaledZ = self->field_004c * g_RayLength;
    float endpointX = self->field_0044 * g_RayLength + matrix[12];
    float endpointY = self->field_0048 * g_RayLength + matrix[13];
    float endpointZ = scaledZ + matrix[14];

    self->field_002c = matrix[12];
    self->field_0030 = matrix[13];
    self->field_0034 = matrix[14];

    CSF_AT(float, self, 0x38) = endpointX;
    CSF_AT(float, self, 0x3C) = endpointY;
    CSF_AT(float, self, 0x40) = endpointZ;

    // Hex-Rays identifies the mask as 255; the machine code loads its global.
    int collisionMask = 255;
    collisionMask = g_RayCollisionMask;
    self->field_0050 = C_0082ddf8_UpdateRayHit(
        self, nullptr, &self->field_0054, &self->field_0058,
        collisionMask);

    char ownsMatrix = CSF_AT(char, matrixResult, 0x4);
    if (ownsMatrix)
        return static_cast<char>(RwMatrixDestroy(
            CSF_AT(std::uint32_t*, matrixResult, 0)));

    return ownsMatrix;
}

HOOK(0x0040F320, C_0082ddf8_UpdateWorldRay, "auto");
