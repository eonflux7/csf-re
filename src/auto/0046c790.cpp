#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x0046C620 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (context), and the second argument repeated.
CSF_FUNC(int, __cdecl, MissionScript_ParseEntryCore, 0x0046C620,
         (int a1, int a2, int a3, int a4));

// 0x0046C790: __thiscall-style forwarder. ECX arrives as the context, the two
// stack arguments are callee-cleaned (ret 8). The original pushes
// (arg2, ECX, arg2, arg1) so the cdecl callee sees (arg1, arg2, ECX, arg2).
static int __fastcall MissionScript_ForwardEntryArguments(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return MissionScript_ParseEntryCore(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x0046C790, MissionScript_ForwardEntryArguments, "auto");
