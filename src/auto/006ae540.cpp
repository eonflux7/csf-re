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

// Scalar computed by 0x6AE4A0 for this object and the int argument; callee pops its stack argument.
CSF_FUNC(double, __fastcall, ComputeMatrixScalarValue, 0x006AE4A0, (void* self, void* edx, int a2));

// Comparison threshold constant (float) stored in the original's data section.
CSF_GLOBAL(float, g_MatrixNearZeroEpsilon, 0x0086009C);

// Returns 1 when the computed scalar is <= the epsilon, otherwise 0.
static int __fastcall IsMatrixScalarNearZero(S_006adb40* self, void* /*edx*/, int a2)
{
    double scalar = ComputeMatrixScalarValue(self, nullptr, a2);
    return scalar <= static_cast<double>(g_MatrixNearZeroEpsilon) ? 1 : 0;
}

HOOK(0x006AE540, IsMatrixScalarNearZero, "auto");
