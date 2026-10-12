#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00468AF0 is __cdecl with four forwarded values: the first stack
// argument, the second stack argument, the incoming ECX (context), and the
// second argument repeated.
CSF_FUNC(int, __cdecl, MissionScript_FreeSlotRange, 0x00468AF0,
         (int* first, int* end, int, int));

// 0x441EC0: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1) so the callee sees (a1, a2, ECX, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall DynamicObj_ForwardSlotRangeWithContext(
    void* context, void* /*edx*/, int* first, int* end)
{
    return MissionScript_FreeSlotRange(
        first,
        end,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        reinterpret_cast<std::uintptr_t>(end) != 0
            ? static_cast<int>(reinterpret_cast<std::uintptr_t>(end))
            : 0);
}

HOOK(0x00441EC0, DynamicObj_ForwardSlotRangeWithContext, "auto");
