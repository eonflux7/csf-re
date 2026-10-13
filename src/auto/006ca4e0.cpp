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

// RwMatrixCreate: [lib], __cdecl, no arguments.
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Plain __stdcall function (listed in a vtable but never reads ecx).
// Stores a freshly created RW matrix in the first dword of the target
// block, sets the flag byte at +4 to 1, and returns the block pointer.
static int __stdcall MatrixSlot_InitWithNewMatrix(int a1)
{
    std::uint32_t* newMatrix = RwMatrixCreate();
    *reinterpret_cast<std::uint32_t*>(a1) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newMatrix));
    *reinterpret_cast<std::uint8_t*>(a1 + 4) = 1;
    return a1;
}

HOOK(0x006CA4E0, MatrixSlot_InitWithNewMatrix, "auto");
