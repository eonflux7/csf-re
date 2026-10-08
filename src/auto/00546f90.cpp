#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Actually invoked with four stack arguments (a1, a2, ecx-at-entry, a2);
// Hex-Rays only recovered two of them.
CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

static int __fastcall ForwardArgumentsToRangeReset(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ResetStringRecordRange(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00546F90, ForwardArgumentsToRangeReset, "auto");
