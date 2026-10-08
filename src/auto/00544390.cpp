#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00544340 consumes the two forwarded stack arguments, the incoming
// ECX (object/context) and the repeated second argument.
CSF_FUNC(int, __cdecl, MenuSaveGame_ProcessArguments, 0x00544340,
         (int* firstArgument, int* secondArgument, std::uintptr_t context, int* repeatedArgument));

// 0x00544390: __stdcall thunk that forwards (arg0, arg1, ECX, arg1) to 0x00544340.
// ECX arrives as the first __fastcall parameter, the two original stack arguments
// become the remaining (callee-cleaned) parameters, so the 8 popped bytes match.
static int __fastcall MenuSaveGame_ForwardArguments(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    return MenuSaveGame_ProcessArguments(
        firstArgument,
        secondArgument,
        reinterpret_cast<std::uintptr_t>(context),
        secondArgument);
}

HOOK(0x00544390, MenuSaveGame_ForwardArguments, "auto");
