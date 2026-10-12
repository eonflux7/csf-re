#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address (signatures per packet).
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_OrientAlongVector, 0x006ADFA0, (S_006ada10* self, void* /*edx*/, void* sourceData));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* /*edx*/, const std::uint32_t* src));

// Builds a copy of the camera's projection matrix in a new RW matrix and
// writes the handle to outAddress: [0] = matrix pointer, [4] = owned flag.
// self is the camera object: orientation block at +0x20, translation block at +0x14.
static int __fastcall CameraProjection_MakeMatrixHandle(void* self, void* /*edx*/, std::uint32_t outAddress)
{
    // Stack-resident temporary: [0] = working matrix pointer, [4] = "needs destroy" flag.
    alignas(8) std::uint8_t holderBytes[8];
    S_006ada10& matrixHolder = *reinterpret_cast<S_006ada10*>(holderBytes);

    matrixHolder.field_0000 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(RwMatrixCreate()));
    CSF_AT(std::uint8_t, holderBytes, 4) = 1;

    GfxMatrix_OrientAlongVector(&matrixHolder, nullptr,
        reinterpret_cast<std::uint8_t*>(self) + 0x20);
    GfxMatrix_CopyTranslationAndUpdate(&matrixHolder, nullptr,
        reinterpret_cast<const std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x14));

    auto* outBlock = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(outAddress));
    std::uint32_t* newMatrix = RwMatrixCreate();
    const void* sourceMatrix = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(matrixHolder.field_0000));

    CSF_AT(std::uint32_t, outBlock, 0) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newMatrix));
    std::memcpy(newMatrix, sourceMatrix, 0x40);
    CSF_AT(std::uint8_t, outBlock, 4) = 1;

    if (CSF_AT(std::uint8_t, holderBytes, 4) != 0) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(matrixHolder.field_0000)));
    }

    return static_cast<int>(outAddress);
}

HOOK(0x006F5DB0, CameraProjection_MakeMatrixHandle, "auto");
