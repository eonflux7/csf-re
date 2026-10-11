#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00515080 is __cdecl. The original pushes four dwords (arg1, incoming ECX,
// arg1, arg0), so the callee receives (arg0, arg1, ECX, arg1) and the caller pops 0x10.
// The packet's bind line lists two parameters, but the caller pushes four, so this
// binding uses four to match the observed call.
CSF_FUNC(int, __cdecl, ProfileSaveGame_ProcessArgumentPair, 0x00515080,
         (int firstArgument, int secondArgument, std::uintptr_t context, int repeatedArgument));

// 0x005151F0: __stdcall thunk with two stack arguments (ret 8). It forwards
// (arg0, arg1, incoming ECX, arg1) to the cdecl callee and returns its result.
// ECX arrives as the first __fastcall parameter; the two stack arguments are
// callee-cleaned, which matches ret 8.
static int __fastcall ProfileSaveGame_ForwardArgumentsThunk(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return ProfileSaveGame_ProcessArgumentPair(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x005151F0, ProfileSaveGame_ForwardArgumentsThunk, "auto");
