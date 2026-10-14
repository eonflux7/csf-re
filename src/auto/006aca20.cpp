#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x6AC7B0 (__thiscall, two stack args, ret 8).
CSF_FUNC(bool, __fastcall, GfxMatrix_ProcessValueAndPointer, 0x006AC7B0,
         (void* self, void* edx, int a2, float* a3));

// The packet's call-site annotation refers to the function, not a struct vtable.
// Code-address annotation: function value, not a vtable.
static const void* const s_MatrixHelperVtable = reinterpret_cast<void**>(CSF_ADDR_FN(0x006AC7B0));

// Forwards the value argument together with a pointer to the parameter slot
// holding that same value. ecx = this is passed through unchanged.
static bool __fastcall GfxMatrix_ForwardValueWithAddress(void* self, void* /*edx*/, int a2)
{
    (void)s_MatrixHelperVtable;
    return GfxMatrix_ProcessValueAndPointer(self, nullptr, a2, reinterpret_cast<float*>(&a2));
}

HOOK(0x006ACA20, GfxMatrix_ForwardValueWithAddress, "auto");
