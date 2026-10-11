#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static void __fastcall CharacterModel_UpdateOffsetTransform(void* self, void* /*edx*/)
{
    if (CSF_AT(std::uint8_t, self, 0x130))
    {
        alignas(8) std::uint8_t vectorBytes[0xC];
        alignas(8) std::uint8_t transformedBytes[0xC];
        alignas(8) std::uint8_t positionBytes[0xC];
        alignas(8) std::uint8_t matrixHandleBytes[0x8];

        float* vector = reinterpret_cast<float*>(vectorBytes);
        float* transformed = reinterpret_cast<float*>(transformedBytes);
        float* position = reinterpret_cast<float*>(positionBytes);

        void* model = CSF_AT(void*, self, 0x4);
        void* transform =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, model, 0)[0x10 / 4])(model, nullptr);

        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, matrixHandleBytes);

        std::uint32_t* matrix =
            CSF_AT(std::uint32_t*, matrixHandleBytes, 0);
        position[0] = CSF_AT(float, matrix, 0x30);
        position[1] = CSF_AT(float, matrix, 0x34);
        position[2] = CSF_AT(float, matrix, 0x38);

        vector[0] = 0.0f;
        vector[1] = 0.0f;
        vector[2] = 0.0f;
        GfxMatrix_CopyTranslationAndUpdate(
            matrixHandleBytes, nullptr,
            reinterpret_cast<const uint32_t*>(vectorBytes));

        vector[0] = 0.0f;
        vector[1] = 0.0f;
        vector[2] = -200.0f;
        transformed[0] = 0.0f;
        transformed[1] = 0.0f;
        transformed[2] = 0.0f;

        RwV3dTransformVectors(
            reinterpret_cast<int>(transformedBytes),
            reinterpret_cast<int>(vectorBytes),
            1,
            reinterpret_cast<int>(
                CSF_AT(std::uint32_t*, matrixHandleBytes, 0)));

        model = CSF_AT(void*, self, 0x4);
        position[0] = transformed[0] + position[0];
        position[1] = transformed[1] + position[1];
        position[2] = transformed[2] + position[2];

        ((void (__fastcall*)(void*, void*, float))
            CSF_AT(void**, model, 0)[0x40 / 4])(
                model, nullptr,
                CSF_AT(float, self, 0x134) + g_Const200_0f);

        model = CSF_AT(void*, self, 0x4);
        transform =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, model, 0)[0x10 / 4])(model, nullptr);
        ((void (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, transform, 0)[0x14 / 4])(
                transform, nullptr, position);

        model = CSF_AT(void*, self, 0x4);
        ((void (__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, model, 0)[0x78 / 4])(model, nullptr, 4, 1);

        if (CSF_AT(std::uint8_t, matrixHandleBytes, 0x4))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixHandleBytes, 0));
    }
    else if (CSF_AT(float, self, 0x134) > g_fZero)
    {
        void* model = CSF_AT(void*, self, 0x4);
        ((void (__fastcall*)(void*, void*, float))
            CSF_AT(void**, model, 0)[0x40 / 4])(
                model, nullptr, CSF_AT(float, self, 0x134));

        model = CSF_AT(void*, self, 0x4);
        ((void (__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, model, 0)[0x78 / 4])(model, nullptr, 4, 0);
    }
}

HOOK(0x004F51B0, CharacterModel_UpdateOffsetTransform, "auto");
