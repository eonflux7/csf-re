#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __cdecl sub_56D3F0(int* a1)  -- rets 0 (cdecl)
CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x56D3F0, (int* a1));

// int __thiscall sub_7082E0(C_00863430* this, int a2)  -- ret 4
// Recomputes the skeletal animation matrices for the hierarchy pointer
// stored at offset +0x04. The stack argument is unused.
static int __fastcall C_00863430_UpdateHAnimHierarchyMatrices(
        C_00863430* self, void* /*edx*/, int /*a2*/)
{
    return RpHAnimHierarchyUpdateMatrices(
        reinterpret_cast<int*>(self->field_0004));
}

HOOK(0x7082E0, C_00863430_UpdateHAnimHierarchyMatrices, "auto");
