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

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(float*, __cdecl, RwMatrixInvert, 0x00624950, (float* a1, float* a2));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));

// Looks up a node by the index returned from the virtual slot at +0x68, then writes the
// node's inverse LTM into the hierarchy's matrix array entry and pushes it to the frame.
static int __fastcall C_00861e98_ApplyFrameInverseToMatrix(C_00861e98* self, void* /*edx*/, int a2)
{
    // Virtual slot 0x68 -> index 26: resolves the node index for a2.
    int nodeIndex = ((int(__fastcall*)(void*, void*, int))self->vtbl[26])(self, nullptr, a2);
    if (nodeIndex == -1)
        return nodeIndex;

    // Hierarchy pointer stored at this+0x14.
    uint32_t hierarchy = CSF_AT(uint32_t, self, 0x14);

    // Node table pointer at hierarchy+0x10; each entry is 16 bytes, node pointer at +0x0C.
    uint32_t nodeTable = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(hierarchy), 0x10);
    uint32_t nodeEntry = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(nodeTable), 16 * nodeIndex + 12);

    // Frame object pointer at nodeEntry+4.
    uint32_t frame = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(nodeEntry), 4);

    // Destination matrix: array base + nodeIndex * 64.
    int matrixArray = RpHAnimHierarchyGetMatrixArray(static_cast<int>(hierarchy));
    int matrixDest = matrixArray + (nodeIndex << 6);

    uint32_t* frameLtm = RwFrameGetLTM(frame);

    alignas(8) uint8_t invertedMatrix[0x40];
    RwMatrixInvert(reinterpret_cast<float*>(invertedMatrix), reinterpret_cast<float*>(frameLtm));
    RwMatrixTransform(reinterpret_cast<uint32_t*>(invertedMatrix),
                      reinterpret_cast<uint32_t*>(matrixDest), 1);

    return RwFrameTransform(static_cast<int>(nodeEntry), reinterpret_cast<int>(invertedMatrix), 0);
}

HOOK(0x006E8CD0, C_00861e98_ApplyFrameInverseToMatrix, "auto");
