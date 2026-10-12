#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotate, 0x006248B0, (uint32_t* a1, float* a2, float a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(void, __fastcall, Math_SphericalInterpolateQuaternion, 0x006AEAC0, (float* self, void* , float* target, float fraction));
CSF_FUNC(void, __fastcall, Matrix_ExtractQuaternion, 0x006AEBC0, (float* self, void* , const void** source));

static uint32_t* __fastcall C_0085e7b8_UpdateWorldRotation(
    C_0085e7b8* self, void* /*edx*/, C_0085e7b8* reference)
{
    alignas(8) uint8_t axisBytes[0x10];

    if (!reference || CSF_AT(int, self, 0xC8) == 3)
    {
        CSF_AT(float, axisBytes, 0x00) = 1.0f;
        CSF_AT(float, axisBytes, 0x04) = 0.0f;
        CSF_AT(float, axisBytes, 0x08) = 0.0f;
        CSF_AT(float, axisBytes, 0x0C) = 1.0f;

        return RwMatrixRotate(
            CSF_AT(uint32_t*, self, 0x40),
            reinterpret_cast<float*>(axisBytes), 90.0f, 0);
    }

    alignas(8) uint8_t rotationQuaternionBytes[0x10];
    alignas(8) uint8_t referenceQuaternionBytes[0x10];
    alignas(8) uint8_t matrixHandleBytes[0x8];
    alignas(8) uint8_t cachedObjectBytes[164];

    GfxMatrix_InitializeQuaternion(
        reinterpret_cast<uint32_t*>(rotationQuaternionBytes), nullptr);

    CSF_AT(float, axisBytes, 0x0C) = 1.0f;
    CSF_AT(uint32_t*, matrixHandleBytes, 0x00) = nullptr;
    CSF_AT(uint32_t, matrixHandleBytes, 0x04) = 0;
    CSF_AT(float, axisBytes, 0x00) = 1.0f;
    CSF_AT(float, axisBytes, 0x04) = 0.0f;
    CSF_AT(float, axisBytes, 0x08) = 0.0f;

    uint32_t* rotationMatrix = RwMatrixCreate();
    RwMatrixRotate(
        rotationMatrix, reinterpret_cast<float*>(axisBytes), 90.0f, 0);

    CSF_AT(uint32_t*, matrixHandleBytes, 0x00) = rotationMatrix;
    CSF_AT(uint8_t, matrixHandleBytes, 0x04) = 0;
    Matrix_ExtractQuaternion(
        reinterpret_cast<float*>(rotationQuaternionBytes), nullptr,
        reinterpret_cast<const void**>(matrixHandleBytes));

    if (CSF_AT(uint8_t, matrixHandleBytes, 0x04))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixHandleBytes, 0x00));

    RwMatrixDestroy(rotationMatrix);

    GfxMatrix_InitializeQuaternion(
        reinterpret_cast<uint32_t*>(referenceQuaternionBytes), nullptr);

    void* provider = GetCachedObjectProvider(reference, nullptr);
    std::memcpy(
        cachedObjectBytes, CSF_AT(const void*, provider, 0x04),
        sizeof(cachedObjectBytes));

    CSF_AT(void*, matrixHandleBytes, 0x00) = cachedObjectBytes + 0x10;
    CSF_AT(uint8_t, matrixHandleBytes, 0x04) = 0;
    Matrix_ExtractQuaternion(
        reinterpret_cast<float*>(referenceQuaternionBytes), nullptr,
        reinterpret_cast<const void**>(matrixHandleBytes));

    if (CSF_AT(uint8_t, matrixHandleBytes, 0x04))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixHandleBytes, 0x00));

    Math_SphericalInterpolateQuaternion(
        reinterpret_cast<float*>(referenceQuaternionBytes), nullptr,
        reinterpret_cast<float*>(rotationQuaternionBytes),
        CSF_AT(float, self, 0x108));

    int matrixResult = GfxMatrix_CreateQuaternionMatrix(
        reinterpret_cast<float*>(referenceQuaternionBytes), nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(matrixHandleBytes)));

    std::memcpy(
        CSF_AT(void*, self, 0x40),
        CSF_AT(const void*, reinterpret_cast<void*>(matrixResult), 0x00),
        0x40u);

    if (CSF_AT(uint8_t, matrixHandleBytes, 0x04))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixHandleBytes, 0x00));

    CSF_AT(uint32_t, CSF_AT(void*, self, 0x40), 0x30) = 0;
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x40), 0x34) = 0;
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x40), 0x38) = 0;
    return nullptr;
}

HOOK(0x006DA140, C_0085e7b8_UpdateWorldRotation, "auto");
