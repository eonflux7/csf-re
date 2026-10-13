#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Copies the 60-byte transform matrix of this object (computed by its
// virtual helpers) into the caller-supplied buffer and releases the
// temporary RW matrix if the lookup flagged one.
static void* __fastcall C_00862900_CopyTransformMatrix(C_00862900* self, void* /*edx*/, void* outMatrix)
{
    alignas(8) std::uint8_t basisBuffer[60];
    alignas(8) std::uint8_t handleBytes[8];

    self->vSlot2C(basisBuffer);
    int matrixHandle = self->vSlot14(reinterpret_cast<int>(handleBytes));
    std::uint32_t* transformed = GfxMatrix_TransformBasisAndPosition(
        reinterpret_cast<std::uint32_t*>(basisBuffer), nullptr,
        reinterpret_cast<std::uint32_t*>(matrixHandle));

    std::uint8_t ownsHandle = handleBytes[4];
    std::memcpy(outMatrix, transformed, 0x3C);

    if (ownsHandle != 0) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(*reinterpret_cast<int*>(handleBytes)));
    }
    return outMatrix;
}

HOOK(0x6F7430, C_00862900_CopyTransformMatrix, "auto");
