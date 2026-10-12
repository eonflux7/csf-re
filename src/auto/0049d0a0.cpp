#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(int, __fastcall, S_00402350_IntersectOrientedBox, 0x006ACB20, (S_00402350* self, void* , const void* box, float* firstDistance, float* secondDistance));

static bool __fastcall C_008342e8_FindIntersectingType20(
    C_008342e8* self, void* /*edx*/, void* source, uint32_t* resultType)
{
    alignas(8) uint8_t firstBoxBytes[0x3C];
    alignas(8) uint8_t secondBoxBytes[0x3C];
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t firstDistanceBytes[0x4] = {};
    alignas(8) uint8_t secondDistanceBytes[0x8] = {};
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t rayBytes[0x1C];

    void* dispatched = C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<void*>(self->field_004c), nullptr);

    std::memset(sphereBytes, 0, sizeof(sphereBytes));

    void* matrixOwner =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, dispatched, 0)[0x34 / 4])(
                dispatched, nullptr);

    float* matrix =
        reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, matrixOwner, 0)[0x1C / 4])(
                matrixOwner, nullptr, reinterpret_cast<float*>(firstBoxBytes));

    GfxMatrix_GetBoundingSphere(
        matrix, nullptr, reinterpret_cast<float*>(sphereBytes));

    void*& entries = CSF_AT(void*, secondDistanceBytes, 0x4);
    entries =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x38 / 4])(source, nullptr);

    uint32_t index = 0;
    uint32_t& byteOffset = CSF_AT(uint32_t, rayBytes, 0x18);
    byteOffset = 0;

    bool hit = false;
    while (index < CSF_AT(uint32_t, entries, 0x8))
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(void*, entries, 0)) + byteOffset;

        if (CSF_AT(uint32_t, entry, 0x8) == 20)
        {
            CSF_AT(float, directionBytes, 0x0) = 0.0f;
            CSF_AT(float, directionBytes, 0x4) = 1000.0f;
            CSF_AT(float, directionBytes, 0x8) = 0.0f;
            NormalizeVector3(
                reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

            CSF_AT(float, rayBytes, 0x0) = 1.0f;
            CSF_AT(float, rayBytes, 0x4) = 0.0f;
            CSF_AT(float, rayBytes, 0x8) = 0.0f;
            CSF_AT(float, rayBytes, 0xC) = 0.0f;
            CSF_AT(float, rayBytes, 0x10) = 0.0f;
            CSF_AT(float, rayBytes, 0x14) = 0.0f;

            uint32_t* position = GetObjectPositionVector(self, nullptr);
            std::memcpy(rayBytes, directionBytes, 0xC);
            std::memcpy(rayBytes + 0xC, position, 0xC);
            NormalizeVector3(
                reinterpret_cast<C_00833b88*>(rayBytes), nullptr);

            void* object = CSF_AT(void*, entry, 0);
            const void* firstBox =
                reinterpret_cast<const void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, object, 0)[0x1C / 4])(
                        object, nullptr, firstBoxBytes);

            S_00402350_IntersectOrientedBox(
                reinterpret_cast<S_00402350*>(rayBytes), nullptr, firstBox,
                reinterpret_cast<float*>(firstDistanceBytes),
                reinterpret_cast<float*>(secondDistanceBytes));

            object = CSF_AT(void*, entry, 0);
            const void* secondBox =
                reinterpret_cast<const void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, object, 0)[0x1C / 4])(
                        object, nullptr, secondBoxBytes);

            hit = S_00402350_IntersectOrientedBox(
                reinterpret_cast<S_00402350*>(rayBytes), nullptr, secondBox,
                reinterpret_cast<float*>(firstDistanceBytes),
                reinterpret_cast<float*>(secondDistanceBytes)) != 0;

            if (hit)
                *resultType = CSF_AT(uint32_t, entry, 0x8);
        }

        ++index;
        byteOffset += 16;
        if (hit)
            break;
    }

    return hit;
}

HOOK(0x0049D0A0, C_008342e8_FindIntersectingType20, "auto");
