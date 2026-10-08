#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x007241E0 - __cdecl handler (rets=[0], so the caller pops its arguments).
// It is called with four stack arguments: the two forwarded pointers, the saved
// caller context (the thunk's incoming ecx) and a duplicate of the second
// pointer.
CSF_FUNC(int*, __cdecl, ResetWideStringRecordRange, 0x007241E0,
         (int* firstArgument, int* secondArgument, std::uintptr_t context, int* repeatedArgument));

// 0x007243D0 - forwards its two pointer arguments together with the incoming
// ecx value (saved as the third argument) and a duplicate of the second
// argument, then returns the handler's result unchanged.
static int* __fastcall ForwardArgumentsAndContextToProcessor(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    return ResetWideStringRecordRange(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x007243D0, ForwardArgumentsAndContextToProcessor, "auto");
