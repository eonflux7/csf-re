#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* /*edx*/, const uint32_t* src));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Copies the frame's translation into a local matrix record, refreshes the
// frame hierarchy, and destroys the matrix when the copy step flagged it.
static char __fastcall C_00860700_UpdateFrameMatrixAndDestroyIfFlagged(C_00860700* self, void* /*edx*/, uint32_t* a2)
{
    // 8-byte local record: [0] = matrix handle (frame field + 0x10), [4] = flag byte.
    alignas(8) uint8_t matrixRecordBytes[8];
    CSF_AT(uint32_t, matrixRecordBytes, 0) = self->field_000c + 0x10;
    CSF_AT(uint8_t, matrixRecordBytes, 4) = 0;

    GfxMatrix_CopyTranslationAndUpdate(matrixRecordBytes, nullptr, a2);
    RwFrameUpdateObjects((int)self->field_000c);

    char result = CSF_AT(uint8_t, matrixRecordBytes, 4);
    if (result != 0) {
        result = (char)RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(uint32_t, matrixRecordBytes, 0)));
    }
    return result;
}

HOOK(0x006BF400, C_00860700_UpdateFrameMatrixAndDestroyIfFlagged, "auto");
