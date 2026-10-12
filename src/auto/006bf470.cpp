#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void*, const uint32_t* src));

// Adds the offset vector a2 to the translation of the frame's matrix (frame + 0x10),
// pushes the result to the graphics matrix, updates the frame's objects, and destroys
// the matrix if the callee raised its flag.
static char __fastcall C_00860700_UpdateTranslatedFrame(C_00860700* self, void* /*edx*/, float* a2)
{
    // The stack object passed to GfxMatrix_CopyTranslationAndUpdate as `this`:
    // +0 holds the matrix pointer, +4 is a flag byte the callee may set.
    alignas(8) uint8_t matrixSlot[8];

    auto* frame = reinterpret_cast<uint8_t*>(self->field_000c);
    CSF_AT(uint32_t, matrixSlot, 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(frame + 0x10));
    CSF_AT(uint8_t, matrixSlot, 4) = 0;

    float translated[3];
    translated[0] = CSF_AT(float, frame, 0x30) + a2[0];
    translated[1] = CSF_AT(float, frame, 0x34) + a2[1];
    translated[2] = CSF_AT(float, frame, 0x38) + a2[2];

    GfxMatrix_CopyTranslationAndUpdate(matrixSlot, nullptr,
        reinterpret_cast<const uint32_t*>(translated));

    RwFrameUpdateObjects(static_cast<int>(self->field_000c));

    char result = static_cast<char>(CSF_AT(uint8_t, matrixSlot, 4));
    if (result) {
        uint32_t matrixAddr = CSF_AT(uint32_t, matrixSlot, 0);
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(static_cast<uintptr_t>(matrixAddr))));
    }
    return result;
}

HOOK(0x006BF470, C_00860700_UpdateTranslatedFrame, "auto");
