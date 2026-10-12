#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(S_00402350*, __fastcall, GfxMatrix_TransformFrame, 0x006AC400, (uint32_t* self, void* , S_00402350* output, int* matrix));
CSF_FUNC(float*, __fastcall, S_006abb80_ExpandBounds, 0x006AC5E0, (S_006abb80* self, void* , S_006abb80* other));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(void, __fastcall, C_00860630_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00860700_SetFrame, 0x006BF380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00860700_GetMatrix, 0x006BF970, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00860700_ConstructEmbeddedState, 0x006BFD00, (void* self, void* edx));

[[maybe_unused]] static const auto kStoredTargetMatrixTable = CSF_VTBL(0x006ADB40);
[[maybe_unused]] static const auto kTransformTable = CSF_VTBL(0x00402350);

static int __cdecl GfxFrames_ComputeClumpLocalBounds(int output, int clump, float flags)
{
    alignas(8) std::uint8_t collectedBounds[0x14];
    CSF_AT(std::uint32_t, collectedBounds, 0x00) = 0;
    CSF_AT(std::uint32_t, collectedBounds, 0x04) = 0;
    CSF_AT(std::uint32_t, collectedBounds, 0x08) = 0;
    CSF_AT(int, collectedBounds, 0x0C) = 10;
    std::memcpy(collectedBounds + 0x10, &flags, 1);

    RpClumpForAllAtomics(
        clump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C0A40)),
        reinterpret_cast<int>(collectedBounds));

    alignas(8) std::uint8_t frameState[0x20];
    C_00860700_ConstructEmbeddedState(frameState, nullptr);
    C_00860700_SetFrame(
        frameState, nullptr,
        CSF_AT(int, reinterpret_cast<void*>(clump), 0x04));

    alignas(8) std::uint8_t inverseMatrix[0x08];
    CSF_AT(float*, inverseMatrix, 0x00) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseMatrix, 0x04) = 1;

    alignas(8) std::uint8_t sourceMatrix[0x08];
    auto* source = static_cast<S_006adb40*>(
        C_00860700_GetMatrix(frameState, nullptr, reinterpret_cast<int>(sourceMatrix)));
    Matrix_InvertWithStoredTarget(
        source, nullptr, reinterpret_cast<float**>(inverseMatrix));

    if (CSF_AT(std::uint8_t, sourceMatrix, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrix, 0x00));

    alignas(8) std::uint8_t accumulatedBounds[0x3C];
    alignas(8) std::uint8_t transformedBounds[0x3C];
    S00402350_InitTransform(
        reinterpret_cast<S_00402350*>(accumulatedBounds), nullptr);

    std::uint32_t index = 0;
    std::uint32_t offset = 0;
    if (CSF_AT(std::uint32_t, collectedBounds, 0x08))
    {
        do
        {
            auto* transformed = GfxMatrix_TransformFrame(
                reinterpret_cast<std::uint32_t*>(
                    CSF_AT(std::uint8_t*, collectedBounds, 0x00) + offset),
                nullptr,
                reinterpret_cast<S_00402350*>(transformedBounds),
                reinterpret_cast<int*>(inverseMatrix));
            S_006abb80_ExpandBounds(
                reinterpret_cast<S_006abb80*>(accumulatedBounds),
                nullptr,
                reinterpret_cast<S_006abb80*>(transformed));
            ++index;
            offset += 60;
        }
        while (index < CSF_AT(std::uint32_t, collectedBounds, 0x08));
    }

    float maxX, minX, maxY, minY, maxZ, minZ;
    S_006abb80_ComputeAxisAlignedBounds(
        reinterpret_cast<S_006abb80*>(accumulatedBounds), nullptr,
        &maxX, &minX, &maxY, &minY, &maxZ, &minZ);

    auto* bounds = reinterpret_cast<void*>(output);
    CSF_AT(float, bounds, 0x0C) = minX;
    CSF_AT(float, bounds, 0x10) = minY;
    CSF_AT(float, bounds, 0x04) = maxY;
    const bool ownsInverse = CSF_AT(std::uint8_t, inverseMatrix, 0x04) != 0;
    CSF_AT(float, bounds, 0x14) = minZ;
    CSF_AT(float, bounds, 0x00) = maxX;
    CSF_AT(float, bounds, 0x08) = maxZ;

    if (ownsInverse)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrix, 0x00));

    C_00860630_DestroyEmbeddedState(frameState, nullptr);
    if (CSF_AT(void*, collectedBounds, 0x00))
        Mem_Free(CSF_AT(void*, collectedBounds, 0x00));

    return output;
}

HOOK(0x006C0C20, GfxFrames_ComputeClumpLocalBounds, "auto");
