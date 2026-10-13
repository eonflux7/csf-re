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

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));

// Returns a2. If the object's delegate at +0x2C exists, calls its slot 35 with the delegate as `this`
// (ecx) and a2 as the single stack argument (thiscall, ret 4), then returns a2.
// Otherwise builds a matrix copy of the transform at +0x30 into the out struct at a2:
//   out[0] = pointer to a fresh 64-byte matrix copy, out[4] = 1.
static int __fastcall C_00862598_FillTransformMatrixOut(C_00862598* self, void* /*edx*/, int a2)
{
    if (self->field_002c != 0)
    {
        void* delegateObject = reinterpret_cast<void*>(self->field_002c);
        using DelegateSlot35Fn = int(__fastcall*)(void* self, void* edx, int a2);
        DelegateSlot35Fn slot35 = reinterpret_cast<DelegateSlot35Fn>(CSF_AT(void**, delegateObject, 0)[35]);
        slot35(delegateObject, nullptr, a2);
        return a2;
    }

    // Local 8-byte holder: [0] = matrix pointer, [4] = owns-matrix flag.
    alignas(8) std::uint8_t holder[8];
    std::uint32_t* sourceMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, holder, 0) = sourceMatrix;
    CSF_AT(std::uint8_t, holder, 4) = 1;
    GfxMatrix_CopyTranslationAndUpdate(
        holder,
        nullptr,
        reinterpret_cast<const uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x30));

    std::uint32_t* newMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t*, reinterpret_cast<std::uint8_t*>(a2), 0) = newMatrix;
    std::memcpy(newMatrix, CSF_AT(std::uint32_t*, holder, 0), 0x40);

    const bool sourceOwned = CSF_AT(std::uint8_t, holder, 4) != 0;
    CSF_AT(std::uint8_t, reinterpret_cast<std::uint8_t*>(a2), 4) = 1;
    if (sourceOwned)
    {
        RwMatrixDestroy(CSF_AT(std::uint32_t*, holder, 0));
    }
    return a2;
}

HOOK(0x006F28D0, C_00862598_FillTransformMatrixOut, "auto");
