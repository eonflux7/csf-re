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

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// __thiscall(this, a2, a3) with ret 8: transforms the point a3 by the object's animation matrix,
// writes the 3-dword result into a2 and returns a2.
static std::uint32_t* __fastcall C_008633a0_TransformPointByAnimMatrix(
    C_008633a0* self, void* /*edx*/, std::uint32_t* outPoint, int inputPoint)
{
    // The slot +0x1C wrapper fills a 2-field block:
    // [0] = pointer to a structure whose first dword is the matrix handle,
    // [4] = ownership flag (char) telling whether the matrix must be destroyed afterwards.
    alignas(8) std::uint8_t matrixBlock[8];
    void* transformRef = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(self->vSlot1C(static_cast<int>(reinterpret_cast<std::uintptr_t>(matrixBlock)))));
    int transformMatrix = CSF_AT(int, transformRef, 0);

    int transformed[3] = {0, 0, 0};
    RwV3dTransformPoints(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(transformed)),
        inputPoint,
        1,
        transformMatrix);

    outPoint[0] = static_cast<std::uint32_t>(transformed[0]);
    outPoint[1] = static_cast<std::uint32_t>(transformed[1]);
    outPoint[2] = static_cast<std::uint32_t>(transformed[2]);

    int matrixHandle = CSF_AT(int, matrixBlock, 0);
    char ownsMatrix = CSF_AT(char, matrixBlock, 4);
    if (ownsMatrix) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(matrixHandle)));
    }

    return outPoint;
}

HOOK(0x00707B50, C_008633a0_TransformPointByAnimMatrix, "auto");
