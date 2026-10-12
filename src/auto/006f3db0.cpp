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

CSF_FUNC(void*, __cdecl, RwFrameSetIdentity, 0x006238C0, (void* frame));
CSF_FUNC(int, __cdecl, RwFrameTranslate, 0x00623830, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(uint32_t*, __fastcall, RpPrtStd_ProcessMatrixState, 0x006F3A30, (void* self, void* edx, int* a2));

// Resets the atomic's dirty counter, rebuilds its frame from the source matrix,
// and runs the particle state update with a temporary matrix that is freed afterwards.
static char __fastcall C_00862748_UpdateFrameAndMatrix(C_00862748* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t frameOwner = self->field_0024;
    self->field_0028 = 0;

    uint32_t frame = CSF_AT(uint32_t, reinterpret_cast<void*>(frameOwner), 4);
    RwFrameSetIdentity(reinterpret_cast<void*>(frame));
    RwFrameTranslate(static_cast<int>(frame), reinterpret_cast<int>(a2), 0);
    RwFrameUpdateObjects(static_cast<int>(frame));

    // Stack frame: [ebp-8] = matrix pointer (4 bytes), [ebp-4] = ownership flag.
    alignas(8) uint8_t frameLocals[8];
    int& matrix = *reinterpret_cast<int*>(frameLocals);
    uint8_t& matrixOwned = frameLocals[4];

    matrix = reinterpret_cast<int>(RwMatrixCreate());
    matrixOwned = 1;
    GfxMatrix_CopyTranslationAndUpdate(frameLocals, nullptr, a2);
    RpPrtStd_ProcessMatrixState(self, nullptr, &matrix);

    char result = static_cast<char>(matrixOwned);
    if (matrixOwned)
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<uint32_t*>(matrix)));
    return result;
}

HOOK(0x006F3DB0, C_00862748_UpdateFrameAndMatrix, "auto");
