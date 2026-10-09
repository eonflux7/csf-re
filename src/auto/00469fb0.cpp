#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00469760 is __cdecl with four forwarded values: the two original
// stack arguments (begin, end), the incoming ECX (owner), and the second argument
// repeated (context).
CSF_FUNC(int, __cdecl, DestroyElementRange, 0x00469760,
         (int* begin, int* end, int* owner, int* context));

// 0x00469FB0: __stdcall thunk. The original pushes (arg0, arg1, ECX, arg1) in that
// order, so the callee sees (arg0, arg1, ECX, arg1). ECX arrives as the first
// __fastcall parameter and the two stack arguments are callee-cleaned (ret 8).
static int __fastcall MissionScript_ForwardElementRangeDestroy(
    void* owner, void* /*edx*/, int* begin, int* end)
{
    return DestroyElementRange(
        begin,
        end,
        reinterpret_cast<int*>(owner),
        end);
}

HOOK(0x00469FB0, MissionScript_ForwardElementRangeDestroy, "auto");
