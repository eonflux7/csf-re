#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x0075D0E0 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (this), and the second argument repeated.
CSF_FUNC(int, __cdecl, WaypointGraph_ProcessArgumentQuad, 0x0075D0E0,
         (int a1, int a2, int a3, int a4));

// 0x0075D390: __thiscall with two stack arguments, callee-cleaned (ret 8).
// Original pushes (arg0, ECX, arg1, arg0) so the callee sees (arg0, arg1, ECX, arg1)
// per the disassembly ordering (a1 lowest on the stack, then a2, this, a2).
static int __fastcall WaypointGraph_ForwardPairArguments(
    void* self, void* /*edx*/, int a1, int a2)
{
    return WaypointGraph_ProcessArgumentQuad(
        a1,
        a2,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        a2);
}

HOOK(0x0075D390, WaypointGraph_ForwardPairArguments, "auto");
