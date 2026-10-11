#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6ADA10, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(int, __cdecl, sub_6ADD90, 0x006ADD90, (void* a1, float* a2, float* a3, int a4));

// Copies the 3-dword block at this->field_0000 + 0x30, zeroes three output buffers,
// fills them from the source object via sub_6ADD90, then passes everything to sub_6ADA10.
static int __fastcall GfxMatrix_ExtractAndApplyTransform(S_006ada10* self, void* /*edx*/, void* sourceData) {
    uint32_t* matrixBlock = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0000 + 48));

    uint32_t inDwords[3];
    inDwords[0] = matrixBlock[0];
    inDwords[1] = matrixBlock[1];
    inDwords[2] = matrixBlock[2];

    float outVecA[3];
    outVecA[0] = 0.0f;
    outVecA[1] = 0.0f;
    outVecA[2] = 0.0f;

    float outVecB[3];
    outVecB[0] = 0.0f;
    outVecB[1] = 0.0f;
    outVecB[2] = 0.0f;

    uint32_t outDwords[3];
    outDwords[0] = 0u;
    outDwords[1] = 0u;
    outDwords[2] = 0u;

    sub_6ADD90(sourceData, outVecA, outVecB,
               static_cast<int>(reinterpret_cast<uintptr_t>(outDwords)));

    return sub_6ADA10(self, nullptr,
                      reinterpret_cast<uint32_t*>(outVecA),
                      reinterpret_cast<uint32_t*>(outVecB),
                      outDwords,
                      inDwords);
}

HOOK(0x006ADF00, GfxMatrix_ExtractAndApplyTransform, "auto");
