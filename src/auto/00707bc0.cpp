#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_InvertInPlace, 0x006ADB10, (void** self, void*));
CSF_FUNC(int, __cdecl, RtQuatConvertFromMatrix, 0x007D9D40, (float* a1, float* a2));

// A matrix pointer followed by its ownership flag (the two dwords the original keeps in a stack slot).
struct OwnedMatrix {
    std::uint32_t* matrix;
    std::uint8_t owns;
};

static char __fastcall GfxAnimInterpolator_UpdateRelativeTransform(
    void* self, void* /*edx*/,
    const void** referenceMatrix, std::uint32_t** adjustmentMatrix)
{
    if (!CSF_AT(float*, self, 0x10))
        return 0;

    OwnedMatrix inverse{};
    inverse.matrix = RwMatrixCreate();
    std::memcpy(inverse.matrix, *referenceMatrix, 0x40);
    inverse.owns = 1;
    GfxMatrix_InvertInPlace(reinterpret_cast<void**>(&inverse), nullptr);

    OwnedMatrix current;  // filled by the indirect getter (out parameter)
    void** currentMatrix = reinterpret_cast<void** (__fastcall*)(
        void*, void*, void*)>(
            CSF_AT(void**, self, 0)[0x14 / 4])(
                self, nullptr, &current);

    std::uint32_t* product = RwMatrixCreate();
    RwMatrixMultiply(product,
                     reinterpret_cast<std::uint32_t*>(*currentMatrix),
                     *adjustmentMatrix);

    std::uint32_t* adjustedCopy = RwMatrixCreate();
    std::memcpy(adjustedCopy, product, 0x40);
    RwMatrixDestroy(product);

    product = RwMatrixCreate();
    RwMatrixMultiply(product, adjustedCopy, inverse.matrix);

    std::uint32_t* relativeMatrix = RwMatrixCreate();
    std::memcpy(relativeMatrix, product, 0x40);
    RwMatrixDestroy(product);
    RwMatrixDestroy(adjustedCopy);

    if (current.owns)
        RwMatrixDestroy(current.matrix);

    RtQuatConvertFromMatrix(
        CSF_AT(float*, self, 0x10),
        reinterpret_cast<float*>(relativeMatrix));

    void* owner = CSF_AT(void*, self, 0x8);
    std::uint32_t node = CSF_AT(std::uint32_t, self, 0xC);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, owner, 0)[0x8C / 4])(owner, nullptr, node);

    CSF_AT(std::uint8_t, self, 0x20) = 1;
    RwMatrixDestroy(relativeMatrix);

    char ownsInverse = static_cast<char>(inverse.owns);
    if (ownsInverse)
        return static_cast<char>(RwMatrixDestroy(inverse.matrix));
    return ownsInverse;
}

HOOK(0x00707BC0, GfxAnimInterpolator_UpdateRelativeTransform, "auto");
