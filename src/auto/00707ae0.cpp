#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// __thiscall(this, a2, a3) with ret 8: writes the transformed vector (3 dwords) into a2 and returns a2.
static std::uint32_t* __fastcall C_008633a0_TransformVectorByAnimMatrix(
    C_008633a0* self, void* /*edx*/, std::uint32_t* outVector, int inputVector)
{
    // Ask the object for its matrix handle. The wrapper fills a 2-field block:
    // [0] = matrix handle (int), [4] = ownership flag (char).
    alignas(8) std::uint8_t matrixBlock[8];
    void* transformRef = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(self->vSlot1C(static_cast<int>(reinterpret_cast<std::uintptr_t>(matrixBlock)))));
    int transformMatrix = CSF_AT(int, transformRef, 0);

    int transformed[3] = {0, 0, 0};
    RwV3dTransformVectors(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(transformed)),
        inputVector,
        1,
        transformMatrix);

    outVector[0] = static_cast<std::uint32_t>(transformed[0]);
    outVector[1] = static_cast<std::uint32_t>(transformed[1]);
    outVector[2] = static_cast<std::uint32_t>(transformed[2]);

    int matrixHandle = CSF_AT(int, matrixBlock, 0);
    char ownsMatrix = CSF_AT(char, matrixBlock, 4);
    if (ownsMatrix) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(matrixHandle)));
    }

    return outVector;
}

HOOK(0x00707AE0, C_008633a0_TransformVectorByAnimMatrix, "auto");
